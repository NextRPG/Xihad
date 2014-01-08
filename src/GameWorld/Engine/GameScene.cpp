#include "GameScene.h"
#include <unordered_map>
#include <iostream>
#include "GameObject.h"
#include "TagListener.h"
#include "ComponentSystem.h"
#include "ComponentSystemRegistry.h"
#include "CppBase\StdMap.h"
#include "GameObjectDepends.h"
#include "ComponentFactory.h"
#include "ComponentSystemFactory.h"
#include "Message\MessageDispatcher.h"
#include "Lua\lua.hpp"
#include "CppBase\XiAssert.h"
#include "RootGameObject.h"

using namespace std;
namespace xihad { namespace ngn
{
	const GameObject::Identifier GameScene::sRootObjectID = "__ROOT__";

	typedef GameObject::Identifier Identifier;
	typedef GameObject::Tag Tag;
	typedef unordered_map<Tag, std::list<GameObject*>> TaggedObjectsMap;
	typedef unordered_map<Identifier, GameObject*> IdentifiedObjectsMap;
	typedef unordered_map<std::string, ComponentSystem*> CompSystemMap;
	struct GameScene::impl
	{
		// systemData can be RenderEngine, PhysicsEngine or AudioEngine;
		int managedObjectId;
		CompSystemMap compSystems;
		GameObjectDepends depends;
		GameScene::Dispatcher* dispatcher;

		GameObject* rootObject;
		IdentifiedObjectsMap sceneObjects;
		TaggedObjectsMap taggedObjects;

		CompositeUpdateHandler& systemUpdater;
		CompositeUpdateHandler& normalUpdater;

		lua_State* mainThread;

		GameScene::ControllerStack controllerStack;

		GameScene::impl() : 
			managedObjectId(0), 
			rootObject(new RootGameObject(&depends, GameScene::sRootObjectID)),
			systemUpdater(*new CompositeUpdateHandler),
			normalUpdater(*new CompositeUpdateHandler)
		{
			sceneObjects[GameScene::sRootObjectID] = rootObject;
		}

		TagListener*& tagListener()
		{
			return depends.listener;
		}

		ComponentFactory*& factory()
		{
			return depends.factory;
		}
	};

	static TagListener* createTagListener(TaggedObjectsMap& taggedObjects)
	{
		class SceneTagListener : public virtual TagListener
		{
		public:
			SceneTagListener(TaggedObjectsMap& objTagMap) : taggedObjects(objTagMap)
			{
			}

			virtual void onTagAdded( GameObject* obj, const std::string& tag ) 
			{
				xassert(obj->hasTag(tag));
				std::list<GameObject*>& objs = taggedObjects[tag];
				objs.push_back(obj);
			}

			virtual void onTagRemoved( GameObject* obj, const std::string& tag ) 
			{
				xassert(!obj->hasTag(tag));
				xassert(taggedObjects.find(tag) != taggedObjects.end());
				std::list<GameObject*>& objs = taggedObjects.at(tag);
				objs.remove(obj);
				if (objs.size() == 0)
				{
					taggedObjects.erase(tag);
				}
			}

		private:
			TaggedObjectsMap& taggedObjects;
		};

		return new SceneTagListener(taggedObjects);
	}

	static ComponentFactory* createComponentFactory(GameScene* scene, CompSystemMap& compSystems)
	{
		class SceneComponentFactory : public virtual ComponentFactory
		{
		public:
			explicit SceneComponentFactory(GameScene* scene, CompSystemMap& systems) : 
				scene(scene), systemMap(systems)
			{
			}

			virtual InheritenceChain hierarchy(const std::string& compName) override
			{
				if (ComponentSystem* sys = scene->requireSystem(compName))
					return sys->hierarchy(compName);
				else
					return InheritenceChain();
			}

			virtual Component* create( const std::string& typeName, 
				GameObject& obj, const Properties& param ) 
			{
				ComponentSystem* system;
				Component* comp = nullptr;
				if (system = scene->requireSystem(typeName))
				{
					comp = system->create(typeName, obj, param);
				}
				return comp;
			}

		private:
			GameScene* scene;
			CompSystemMap& systemMap;
		};

		return new SceneComponentFactory(scene, compSystems);
	}

	GameScene::GameScene(lua_State* L) : mImpl(new impl)
	{
		mImpl->tagListener() = createTagListener(mImpl->taggedObjects);
		mImpl->factory() = createComponentFactory(this, mImpl->compSystems);
		mImpl->depends.scene = this;

		if (L == nullptr)
		{
			L = luaL_newstate();
			luaL_openlibs(L);
		}
		else 
		{
			// check if the param thread is a main thread
			if (lua_pushthread(L) != 1)
				cout << "WARNING: globalState is not the main thread"  << endl;
			lua_pop(L, 1);
		}

		mImpl->mainThread = L;
		mImpl->dispatcher = new MessageDispatcher<GameObject, GameScene, MessageListener>(*this);
		mImpl->systemUpdater.appendUpdateHandler(mImpl->dispatcher);
		this->appendUpdateHandler(&mImpl->systemUpdater);
		this->appendUpdateHandler(mImpl->rootObject);
	}

	GameScene::~GameScene()
	{
		delete mImpl->factory();
		delete mImpl->tagListener();

		lua_State* L = mImpl->mainThread;
		delete mImpl;
		lua_close(L);
	}

	lua_State* GameScene::mainThread() const
	{
		return mImpl->mainThread;
	}

	GameObject* GameScene::rootObject() const
	{
		return mImpl->rootObject;
	}
	
	GameScene::Dispatcher* GameScene::dispatcher() const
	{
		return mImpl->dispatcher;
	}

	GameObject* GameScene::createUniqueObject( GameObject::IdArgType header, GameObject* parent, bool addToTag )
	{
		string uniqueId = header;
		(uniqueId += "#") += to_string(mImpl->managedObjectId);

		GameObject* created = createObject(uniqueId, parent);
		if (addToTag && created != nullptr)
			created->addTag(header);

		return created;
	}

	GameObject* GameScene::createObject( GameObject::IdArgType id, GameObject* parent )
	{
		if (parent && parent->getScene() != this || 
			StdMap::containsKey(mImpl->sceneObjects, id))
			return nullptr;

		GameObject* obj = new GameObject(&mImpl->depends, id);
		obj->setParent(parent ? parent : rootObject());

		++mImpl->managedObjectId;
		mImpl->sceneObjects[id] = obj;

		return obj;
	}

	GameObject* GameScene::createObject( GameObject::IdArgType id, GameObject::IdArgType pid )
	{
		if (GameObject* parentObject = findObject(pid))
			return createObject(id, parentObject);
		else 
			return nullptr;
	}

	bool GameScene::destroyObject( GameObject::IdArgType id )
	{
		GameObject* obj = findObject(id);
		if (obj != nullptr && obj->getScene() == this)
		{
			return obj->destroy();
		}

		return false;
	}

	ComponentSystem* GameScene::requireSystem( const std::string& systemName )
	{
		ComponentSystemFactory* factory;
		ComponentSystem* sys = StdMap::findPtrValue(mImpl->compSystems, systemName);

		if (!sys &&
			(factory = ComponentSystemRegistry::findSystem(systemName)) &&
			(sys = factory->create(this, systemName)))
		{
			mImpl->compSystems[systemName] = sys;
			mImpl->systemUpdater.appendUpdateHandler(sys);
		}

		return sys;
	}

	GameObject* GameScene::findObject( GameObject::IdArgType id )
	{
		return StdMap::findPtrValue(mImpl->sceneObjects, id);
	}

	const GameScene::ObjectGroup* GameScene::findObjectsByTag( const std::string& tag )
	{
		auto res = StdMap::findValuePtr(mImpl->taggedObjects, tag);

		if (!res || res->empty())
			return nullptr;
		else 
			return res;
	}

	void GameScene::onObjectDestroyed( GameObject* obj )
	{
		if (obj == nullptr) return;

		if (obj->getScene() != this)
		{
			cerr << "GameObject passed to GameScene is not correct" << endl;
			return;
		}

		obj->clearTags();
		XiAssert::areEqual(1U, mImpl->sceneObjects.erase(obj->getID()));

		if (obj == mImpl->rootObject)
		{
			this->removeUpdateHandler(mImpl->rootObject);
			mImpl->rootObject = nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Event Process
	template <typename iterator, typename Event>
	inline static bool spreadEvent(iterator bgn, iterator end, const Event& e)
	{
		while (bgn != end)
		{
			if ((*bgn)->onBackgroundEvent(e))
				return true;
			++bgn;
		}

		return false;
	}

	template <typename Event>
	inline static bool deliverEvent(GameScene::ControllerStack& stack, const Event& e )
	{
		auto receiverIter = stack.rbegin();
		if (receiverIter == stack.rend())
			return false;
		else if ((*receiverIter)->onForegroundEvent(e))
			return true;
		else 
			++receiverIter;

		return spreadEvent(receiverIter, stack.rend(), e);
	}

	bool GameScene::onForegroundEvent( const KeyEvent& event )
	{
		return deliverEvent(mImpl->controllerStack, event);
	}

	bool GameScene::onForegroundEvent( const MouseEvent& event )
	{
		return deliverEvent(mImpl->controllerStack, event);
	}

	bool GameScene::onBackgroundEvent( const KeyEvent& event )
	{
		auto& stack = mImpl->controllerStack;
		return spreadEvent(stack.rbegin(), stack.rend(), event);
	}

	bool GameScene::onBackgroundEvent( const MouseEvent& event )
	{
		auto& stack = mImpl->controllerStack;
		return spreadEvent(stack.rbegin(), stack.rend(), event);
	}

	GameScene::ControllerStack& GameScene::controllerStack() const
	{
		return mImpl->controllerStack;
	}

}}

