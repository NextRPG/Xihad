#include "GameObject.h"
#include <bitset>
#include <unordered_map>
#include <boost/cast.hpp>
#include "Component.h"
#include "Transform.h"
#include "ComponentSystemRegistry.h"
#include "CppBase/StdMap.h"
#include "ComponentFactory.h"
#include "TagManager.h"
#include "GameObjectDepends.h"
#include "CppBase/xassert.h"
#include "GameScene.h"
#include "MemoryLeakDetector.h"
#include <iostream>
#include "GameObjectVisitor.h"


template <typename T>
inline static void clearBits(T* bits, T mask)
{
	*bits &= ~mask;
}

using namespace std;
using namespace boost;
using namespace irr;
using namespace core;
namespace xihad { namespace ngn
{
	using std::string;
	typedef GameObjectDepends::TagManagerT MyTagManager;

	enum { MaxTagCount = MyTagManager::MaxTagCount };
	enum 
	{
		TRANSLATE_DIRTY = 1,
		ROTATE_DIRTY = 2,
		SCALE_DIRTY = 4,
		LOCAL_DIRTY = TRANSLATE_DIRTY|ROTATE_DIRTY|SCALE_DIRTY,
		PARENT_DIRTY = 8,
	};

	enum
	{
		IDENTITY_PARENT = 1,
	};

	struct GameObject::impl
	{
		GameObjectDepends* depends;
		Identifier objectID;
		bitset<MaxTagCount> tags;

		GameObject* parent;
		CompositeUpdateHandler* childrenList;	// managed by GameObject

		mutable short hints;
		mutable short transfromHints;
		mutable Matrix localMatrix;
		mutable Matrix worldMatrix;
		mutable Transform worldTransform;
		Transform localTransform;
#ifdef XIHAD_CACHE_ROTATE_MATRIX
		mutable Matrix rotateMatrix;
#endif

		unordered_map<string, Component*> components;

		impl(GameObjectDepends* pDepends, IdArgType id) :
			depends(pDepends), objectID(id), parent(nullptr), childrenList(0), 
			hints(IDENTITY_PARENT), transfromHints(LOCAL_DIRTY|PARENT_DIRTY)
		{
		}

		bool identityParent() const
		{
			return hints & IDENTITY_PARENT;
		}

		void setParent(GameObject* newParent)
		{
			parent = newParent;

			// update hints
			if (newParent == scene()->getRootObject() || newParent == nullptr)
				hints |= IDENTITY_PARENT;
			else
				clearBits(&hints,(short) IDENTITY_PARENT);
		}

		CompositeUpdateHandler* notNullChildrenList(GameObject* self)
		{
			if (!childrenList)
			{
				childrenList = new CompositeUpdateHandler;
				if (self->isUpdating())
					childrenList->start();
			}

			return childrenList;
		}

		bool hasChildHandler() const
		{
			return childrenList && childrenList->hasChildHandler();
		}

		inline MyTagManager* tagListener() 
		{
			return depends->tagManager;
		}

#define ADD_IF_TAG_NOT_EXSITS true
		unsigned getTagIndex(const GameObject::Tag& tag, bool add = false)
		{
			return depends->tagManager->unprojectTag(tag, add);
		}

		inline ComponentFactory* factory()
		{
			return depends->factory;
		}

		inline GameScene::Dispatcher* dispatcher()
		{
			return scene()->getDispatcher();
		}

		inline GameScene* scene()
		{
			return depends->scene;
		}
	};

	GameObject::GameObject( GameObjectDepends* depends, IdArgType id )
	{
		xassert(depends);
		xassert(depends->tagManager);
		xassert(depends->factory);

		mImpl.reset(new impl(depends, id));

		XIHAD_MLD_NEW_OBJECT;
	}

	/************************************************************************/
	/* delete all components created by this object, and this operation will
	 * delete all children objects belong to this object
	/************************************************************************/
	GameObject::~GameObject()
	{
		getScene()->onObjectDestroyed(this);

		XIHAD_MLD_DEL_OBJECT;
	}

	GameScene* GameObject::getScene() const
	{
		return mImpl->scene();
	}

	Component* GameObject::findComponent( const string& componentName )
	{
		return StdMap::findPtrValue(mImpl->components, componentName);
	}

	Component* GameObject::findComponentHavingSameRoot( const string& componentName )
	{
		auto factory = mImpl->factory();
		auto ancestors = factory->hierarchy(componentName);
		Component* res = nullptr;

		if (ancestors.begin() != ancestors.end())
		{
			const string& rootClass = ancestors.root();
			res = findComponent(rootClass);
		}
		
		return res;
	}

	pair<Component*, bool> GameObject::appendComponent( 
		const string& typeName, const Properties& specificParam )
	{
		auto result = make_pair(findComponentHavingSameRoot(typeName), false);

		if (result.first == nullptr)
		{
			ComponentFactory* factory = mImpl->factory();
			if (result.first = factory->create(typeName, *this, specificParam)) // create new?
			{
				result.second = true;
				InheritancePath ancestors = factory->hierarchy(typeName);

				for (const string& superclass : ancestors)
					mImpl->components[superclass] = result.first;

				appendChildHandler(result.first);
			}
		}
			

		return result;
	}

	bool GameObject::removeComponent( const std::string& componentName )
	{
		Component* comp;
		if (comp = StdMap::findPtrValue(mImpl->components, componentName))
		{
			auto facotry = mImpl->factory();
			const string& mostDerived = comp->getComponentName();
			InheritancePath ancestors = facotry->hierarchy(mostDerived);
			for (const string& clazz : ancestors)
			{
				int sz = mImpl->components.erase(clazz);
				xassert(sz == 1);
			}

			return destroyChildHandler(comp);
		}
		
		return false;
	}

	bool GameObject::hasTag( TagArgType tag ) const
	{
		unsigned tidx = mImpl->getTagIndex(tag);
		if (tidx == MyTagManager::InvalidIndex)
			return false;
		else 
			return mImpl->tags.test(tidx);
	}

	void GameObject::addTag( TagArgType tag )
	{
		unsigned tidx = mImpl->getTagIndex(tag, ADD_IF_TAG_NOT_EXSITS);
		if (!mImpl->tags.test(tidx))
		{
			mImpl->tags.set(tidx);
			mImpl->tagListener()->onTagAdded(this, tidx);
		}
	}

	void GameObject::removeTag( TagArgType tag )
	{
		unsigned tidx = mImpl->getTagIndex(tag);
		if (tidx != MyTagManager::InvalidIndex && mImpl->tags.test(tidx))
		{
			mImpl->tags.reset(tidx);
			mImpl->tagListener()->onTagRemoved(this, tidx);
		}
	}

	void GameObject::clearTags()
	{
		for (unsigned idx = 0; idx < MyTagManager::MaxTagCount; ++idx)
			if (mImpl->tags.test(idx))
			{
				mImpl->tags.reset(idx);
				mImpl->tagListener()->onTagRemoved(this, idx);
			}
	}

	GameObject::IdArgType GameObject::getID() const
	{
		return mImpl->objectID;
	}

	void GameObject::updateChildrenWorldMatrix()
	{
		if (mImpl->transfromHints || !mImpl->hasChildHandler())
			return;

		xassert(mImpl->childrenList);
		CompositeUpdateHandler::iterator it = mImpl->childrenList->childHandlerBegin();
		while (it != mImpl->childrenList->childHandlerEnd())
		{
			if (GameObject* go = polymorphic_downcast<GameObject*>(*it))
			{
				go->updateChildrenWorldMatrix();
				go->mImpl->transfromHints |= PARENT_DIRTY;
			}
			else
			{
				cerr << "Non-GameObject updater found" << endl;
				xassert(false);
			}

			++it;
		}
	}

	void GameObject::resetScale( const vector3df& scale )
	{
		const vector3df& mScale = mImpl->localTransform.getScale();
		if (mScale != scale)
		{
			mImpl->localTransform.resetScale(scale);
			updateChildrenWorldMatrix();
			mImpl->transfromHints |= SCALE_DIRTY;
		}
	}

	void GameObject::resetRotate( const vector3df& rotate )
	{
		mImpl->localTransform.resetRotate(rotate);
		updateChildrenWorldMatrix();
		mImpl->transfromHints |= ROTATE_DIRTY;		
	}

	void GameObject::resetTranslate( const vector3df& trans )
	{
		mImpl->localTransform.resetTranslate(trans);
		updateChildrenWorldMatrix();
		mImpl->transfromHints |= TRANSLATE_DIRTY;
	}

	void GameObject::updateWorldMatrix() const
	{
		auto& base = mImpl->parent->getWorldTransformMatrix();

		// update local matrix
		mImpl->worldMatrix.setbyproduct(base, getLocalTransformMatrix());
		mImpl->transfromHints = 0;
	}

	const Matrix& GameObject::getWorldTransformMatrix() const
	{
		if (mImpl->identityParent())
		{
			const Matrix& ret = getLocalTransformMatrix();
			mImpl->transfromHints = 0;
			return ret;
		}

		if (mImpl->transfromHints)
			updateWorldMatrix();

		return mImpl->worldMatrix;
	}

	const Transform& GameObject::getWorldTransform() const
	{
		if (mImpl->identityParent())
			return mImpl->localTransform;

		if (mImpl->transfromHints)
		{
			updateWorldMatrix();
			mImpl->worldTransform.resetTranslate(mImpl->worldMatrix.getTranslation());
			mImpl->worldTransform.resetScale(mImpl->worldMatrix.getScale());

			if (mImpl->transfromHints & (PARENT_DIRTY|ROTATE_DIRTY))
				mImpl->worldTransform.setFromMatrix(mImpl->worldMatrix);
		}

		return mImpl->worldTransform;
	}

	const Matrix& GameObject::getLocalTransformMatrix() const
	{
		if (mImpl->transfromHints & LOCAL_DIRTY)
		{
#ifdef XIHAD_CACHE_ROTATE_MATRIX
			if (mImpl->transfromHints & ROTATE_DIRTY)
				mImpl->rotateMatrix.setRotationDegrees(getRotation());

			mImpl->localMatrix = mImpl->rotateMatrix;
#else
			mImpl->localMatrix.setRotationDegrees(getRotation());
#endif

			mImpl->localMatrix.setTranslation(getTranslate());
			vector3df vscale = mImpl->localTransform.getScale();
			float mScale[] = { vscale.X, vscale.Y, vscale.Z };
			for (int idx = 0, first = 0; first < 12; first += 4, ++idx)
			{
				mImpl->localMatrix[first+0] *= mScale[idx];
				mImpl->localMatrix[first+1] *= mScale[idx];
				mImpl->localMatrix[first+2] *= mScale[idx];
			}

			clearBits(&mImpl->transfromHints, (short)LOCAL_DIRTY);
		}

		return mImpl->localMatrix;
	}

	bool GameObject::identityParent() const
	{
		return mImpl->identityParent();
	}

	void GameObject::setParent( GameObject* newParent )
	{
		GameObject* oldParent = mImpl->parent;
		if (oldParent == newParent) 
			return;

		mImpl->setParent(newParent);

		// unlink
		if (oldParent != nullptr)
		{
			CompositeUpdateHandler* list = oldParent->mImpl->childrenList;
			xassert(list);
			list->eraseChildHandler(list->findChildHandler(this));
		}

		// link
		if (newParent != nullptr)
			newParent->mImpl->notNullChildrenList(this)->appendChildHandler(this);

		mImpl->transfromHints |= PARENT_DIRTY;
	}

	GameObject* GameObject::getParent() const
	{
		return mImpl->parent;
	}

	void GameObject::visitChildren( GameObjectVisitor& visitor )
	{
		if (mImpl->hasChildHandler())
		{
			auto it = mImpl->childrenList->childHandlerBegin();
			while (it != mImpl->childrenList->childHandlerEnd())
			{
				GameObject* go = static_cast<GameObject*>(*it);
				visitor.onVisitObject(*go);
				++it;
			}
		}
	}

	void GameObject::onStart()
	{
		// Let self start first. Order is reserved
		CompositeUpdateHandler::onStart();

		if (mImpl->hasChildHandler())
			mImpl->childrenList->start();
	}

	void GameObject::onUpdate( const Timeline& tm )
	{
		// Order is reserved
		CompositeUpdateHandler::onUpdate(tm);

		// There are at least half game objects having no child, therefore we can benefit from the test
		if (mImpl->hasChildHandler())
			mImpl->childrenList->update(tm);
	}

	void GameObject::onStop()
	{
		// Order is reserved
		if (mImpl->hasChildHandler())
			mImpl->childrenList->stop();

		CompositeUpdateHandler::onStop();
	}

	void GameObject::onDestroy()
	{
		// Order is reserved
		if (mImpl->hasChildHandler() && !mImpl->childrenList->destroy())
			cerr << "GameObject children list destroy failed" << endl;

		mImpl->childrenList = 0;
		CompositeUpdateHandler::onDestroy();
	}

	vector3df GameObject::getScale() const
	{
		return mImpl->localTransform.getScale();
	}

	vector3df GameObject::getRotation() const
	{
		return mImpl->localTransform.getRotation();
	}

	vector3df GameObject::getTranslate() const
	{
		return mImpl->localTransform.getTranslation();
	}

}}

