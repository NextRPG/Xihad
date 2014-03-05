#include "GameObject.h"
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <boost/cast.hpp>
#include "Component.h"
#include "Transform.h"
#include "ComponentSystemRegistry.h"
#include "CppBase/StdMap.h"
#include "ComponentFactory.h"
#include "TagListener.h"
#include "GameObjectDepends.h"
#include "CppBase/xassert.h"
#include "GameScene.h"

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

	using std::string;
	struct GameObject::impl
	{
		GameObjectDepends* depends;
		Identifier objectID;

		GameObject* parent;
		CompositeUpdateHandler* childrenList;	// managed by GameObject

		mutable int transfromDirtyBits;
		mutable Matrix rotateMatrix;
		mutable Matrix localMatrix;
		mutable Matrix worldMatrix;

		unordered_set<GameObject::Tag> tags;
		unordered_map<string, Component*> components;

		impl(GameObjectDepends* pDepends, IdArgType id) :
			depends(pDepends), objectID(id), parent(nullptr), 
			childrenList(new CompositeUpdateHandler),
			transfromDirtyBits(LOCAL_DIRTY|PARENT_DIRTY)
		{
		}

		inline TagListener* listener() 
		{
			return depends->listener;
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
		xassert(depends->listener);
		xassert(depends->factory);

		mImpl.reset(new impl(depends, id));
	}

	/************************************************************************/
	/* delete all components created by this object, and this operation will
	 * delete all children objects belong to this object
	/************************************************************************/
	GameObject::~GameObject()
	{
		getScene()->onObjectDestroyed(this);
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
				InheritenceChain ancestors = factory->hierarchy(typeName);

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
			InheritenceChain ancestors = facotry->hierarchy(mostDerived);
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
		return mImpl->tags.find(tag) != mImpl->tags.end();
	}

	void GameObject::addTag( TagArgType tag )
	{
		if (mImpl->tags.insert(tag).second) // insert success
			mImpl->listener()->onTagAdded(this, tag);
	}

	void GameObject::removeTag( TagArgType tag )
	{
		if (mImpl->tags.erase(tag))
			mImpl->listener()->onTagRemoved(this, tag);
	}

	void GameObject::clearTags()
	{
		for (auto it = mImpl->tags.begin(); it != mImpl->tags.end(); )
		{
			string tag = *it;
			it = mImpl->tags.erase(it);
			mImpl->listener()->onTagRemoved(this, tag);
		}
	}

	GameObject::IdArgType GameObject::getID() const
	{
		return mImpl->objectID;
	}

	void GameObject::updateChildrenWorldMatrix()
	{
		if (mImpl->transfromDirtyBits ||	// already updated?
			mImpl->childrenList->getChildHandlerCount() == 0)  // no child?	
			return;

		CompositeUpdateHandler::iterator it = mImpl->childrenList->childHandlerBegin();
		while (it != mImpl->childrenList->childHandlerEnd())
		{
			if (GameObject* go = polymorphic_downcast<GameObject*>(*it))
			{
				go->mImpl->transfromDirtyBits |= PARENT_DIRTY;
				go->updateChildrenWorldMatrix();
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
			{
				mImpl->rotateMatrix.setRotationDegrees(getRotation());
			}

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
			list->eraseChildHandler(list->findChildHandler(this));
		}

		// link
		if (newParent != nullptr)
			newParent->mImpl->childrenList->appendChildHandler(this);

		mImpl->transfromDirtyBits |= PARENT_DIRTY;
	}

	GameObject* GameObject::getParent() const
	{
		return mImpl->parent;
	}

	GameObject::child_iterator GameObject::firstChild() const
	{
		auto it = mImpl->childrenList->childHandlerBegin();
		return *reinterpret_cast<child_iterator*>(&it);
	}

	GameObject::child_iterator GameObject::lastChild() const
	{
		auto it = mImpl->childrenList->childHandlerEnd();
		return *reinterpret_cast<child_iterator*>(&it);
	}

	void GameObject::onStart()
	{
		// Let self start first. Order is reserved
		CompositeUpdateHandler::onStart();
		mImpl->childrenList->start();
	}

	void GameObject::onUpdate( const Timeline& tm )
	{
		// Order is reserved
		CompositeUpdateHandler::onUpdate(tm);
		mImpl->childrenList->update(tm);
	}

	void GameObject::onStop()
	{
		// Order is reserved
		mImpl->childrenList->stop();
		CompositeUpdateHandler::onStop();
	}

	void GameObject::onDestroy()
	{
		// Order is reserved
		mImpl->childrenList->destroy();
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

