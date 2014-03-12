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

using namespace std;
using namespace boost;
using namespace irr;
using namespace core;
namespace xihad { namespace ngn
{
	enum 
	{
		TRANSLATE_DIRTY = 1,
		ROTATE_DIRTY = 2,
		SCALE_DIRTY = 4,
		LOCAL_DIRTY = TRANSLATE_DIRTY|ROTATE_DIRTY|SCALE_DIRTY,
		PARENT_DIRTY = 8,
	};

	typedef GameObjectDepends::TagManagerT MyTagManager;
	using std::string;
	struct GameObject::impl
	{
		GameObjectDepends* depends;
		Identifier objectID;
		bitset<MyTagManager::MaxTagCount> tags;

		GameObject* parent;
		CompositeUpdateHandler* childrenList;	// managed by GameObject

		mutable int transfromDirtyBits;
		mutable Matrix rotateMatrix;
		mutable Matrix localMatrix;
		mutable Matrix worldMatrix;

		unordered_map<string, Component*> components;

		impl(GameObjectDepends* pDepends, IdArgType id) :
			depends(pDepends), objectID(id), parent(nullptr), 
			childrenList(0), transfromDirtyBits(LOCAL_DIRTY|PARENT_DIRTY)
		{
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
		if (mImpl->transfromDirtyBits || !mImpl->hasChildHandler())
			return;

		xassert(mImpl->childrenList);
		CompositeUpdateHandler::iterator it = mImpl->childrenList->childHandlerBegin();
		while (it != mImpl->childrenList->childHandlerEnd())
		{
			if (GameObject* go = polymorphic_downcast<GameObject*>(*it))
			{
				go->updateChildrenWorldMatrix();
				go->mImpl->transfromDirtyBits |= PARENT_DIRTY;
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
		const vector3df& mScale = mTransform.getScale();
		if (mScale != scale)
		{
			mTransform.resetScale(scale);
			updateChildrenWorldMatrix();
			mImpl->transfromDirtyBits |= SCALE_DIRTY;
		}
	}

	void GameObject::resetRotate( const vector3df& rotate )
	{
		mTransform.resetRotate(rotate);
		updateChildrenWorldMatrix();
		mImpl->transfromDirtyBits |= ROTATE_DIRTY;		
	}

	void GameObject::resetTranslate( const vector3df& trans )
	{
		mTransform.resetTranslate(trans);
		updateChildrenWorldMatrix();
		mImpl->transfromDirtyBits |= TRANSLATE_DIRTY;
	}

	inline static void clearBits(int* bits, int mask)
	{
		*bits &= ~mask;
	}

	const Matrix& GameObject::getWorldTransformMatrix() const
	{
		if (identityParent())
		{
			const Matrix& ret = getLocalTransformMatrix();
			mImpl->transfromDirtyBits = 0;
			return ret;
		}

		if (mImpl->transfromDirtyBits)
		{
			auto& base = mImpl->parent->getWorldTransformMatrix();

			// update local matrix
			mImpl->worldMatrix = base * getLocalTransformMatrix();
			mImpl->transfromDirtyBits = 0;
		}

		return mImpl->worldMatrix;
	}

	const Matrix& GameObject::getLocalTransformMatrix() const
	{
		if (mImpl->transfromDirtyBits & LOCAL_DIRTY)
		{
			if (mImpl->transfromDirtyBits & ROTATE_DIRTY)
				mImpl->rotateMatrix.setRotationDegrees(getRotation());

			mImpl->localMatrix = mImpl->rotateMatrix;
			mImpl->localMatrix.setTranslation(getTranslate());

			vector3df vscale = getScale();
			float mScale[] = { vscale.X, vscale.Y, vscale.Z };
			for (int idx = 0, first = 0; 
				 first < 12; 
				 first += 4, ++idx)
			{
				mImpl->localMatrix[first+0] *= mScale[idx];
				mImpl->localMatrix[first+1] *= mScale[idx];
				mImpl->localMatrix[first+2] *= mScale[idx];
			}

			clearBits(&mImpl->transfromDirtyBits, LOCAL_DIRTY);
		}

		return mImpl->localMatrix;
	}

	bool GameObject::identityParent() const
	{
		return  mImpl->parent == nullptr || 
				mImpl->parent == mImpl->scene()->getRootObject();
	}

	void GameObject::setParent( GameObject* newParent )
	{
		GameObject* oldParent = mImpl->parent;
		if (oldParent == newParent) 
			return;

		mImpl->parent = newParent;

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

		mImpl->transfromDirtyBits |= PARENT_DIRTY;
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
		return mTransform.getScale();
	}

	vector3df GameObject::getRotation() const
	{
		return mTransform.getRotation();
	}

	vector3df GameObject::getTranslate() const
	{
		return mTransform.getTranslation();
	}

}}

