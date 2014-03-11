#include "GameScene.h"

// use in member field
#include <unordered_map>
#include "xptr.h"
#include "TagListener.h"
#include "RootGameObject.h"
#include "GameObjectDepends.h"
#include "ComponentSystem.h"
#include "ComponentFactory.h"
#include "UserEventReceiverStack.h"
#include "Message\MessageDispatcher.h"

// use in implementation
#include "ComponentSystemRegistry.h"
#include "ComponentSystemFactory.h"
#include "CppBase\StdMap.h"
#include "CppBase\XiAssert.h"
#include <iostream>

using namespace std;
namespace xihad { namespace ngn
{
	const GameObject::Identifier GameScene::sRootObjectID = "root";

	typedef GameObject::Identifier Identifier;
	typedef GameObject::Tag Tag;
	typedef unordered_map<Tag, std::list<GameObject*>> TaggedObjectsMap;
	typedef unordered_map<Identifier, GameObject*> IdentifiedObjectsMap;
	typedef unordered_map<std::string, ComponentSystem*> CompSystemMap;

	struct GameScene::impl : public GameObjectDepends
	{
		// 1st child updater in GameScene
		GameScene::Dispatcher* dispatcher;

		// 2nd child updater in GameScene
		CompositeUpdateHandler* systemUpdater;
		CompSystemMap compSystems;

		// 3rd child updater in GameScene
		GameObject* rootObject;
		IdentifiedObjectsMap sceneObjects;
		TaggedObjectsMap taggedObjects;
		int managedObjectId;

		UserEventReceiverStack* receiverStack;

		GameScene::impl() : dispatcher(nullptr),
			systemUpdater(new CompositeUpdateHandler),
			rootObject(0), managedObjectId(0), 
			receiverStack(new UserEventReceiverStack)
		{
			sceneObjects[sRootObjectID] = rootObject;
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
		class SceneComponentFactory : public ComponentFactory
		{
		public:
			explicit SceneComponentFactory(GameScene* scene, CompSystemMap& systems) : 
				scene(scene), systemMap(systems)
			{
			}

			virtual InheritancePath hierarchy(const std::string& compName) override
			{
				if (ComponentSystem* sys = scene->requireSystem(compName))
					return sys->hierarchy(compName);
				else
					return InheritancePath();
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

	GameScene::GameScene() : mImpl(new impl)
	{
		XIHAD_MLD_NEW_OBJECT;

		mImpl->listener = createTagListener(mImpl->taggedObjects);
		mImpl->factory = createComponentFactory(this, mImpl->compSystems);
		mImpl->scene = this;

		// 1st
		mImpl->dispatcher = new MessageDispatcher<GameObject, GameScene, MessageListener>(*this);
		this->appendChildHandler(mImpl->dispatcher);

		// 2nd
		this->appendChildHandler(mImpl->systemUpdater);

		// 3rd
		mImpl->rootObject = new RootGameObject(mImpl.get(), sRootObjectID);
		this->appendChildHandler(mImpl->rootObject);

		// Ok, wait for the rest updater
	}

	GameScene::~GameScene()
	{
		// clear mImpl->depends
		delete mImpl->factory;
		delete mImpl->listener;
		mImpl->scene = 0;

		XIHAD_MLD_DEL_OBJECT;
	}

	GameObject* GameScene::getRootObject() const
	{
		return mImpl->rootObject;
	}
	
	GameScene::Dispatcher* GameScene::getDispatcher() const
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

		GameObject* obj = new GameObject(mImpl.get(), id);
		obj->setParent(parent ? parent : getRootObject());

		++mImpl->managedObjectId;
		mImpl->sceneObjects[id] = obj;

		return obj;
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
			if (!mImpl->systemUpdater->containsChildHandler(sys))
				mImpl->systemUpdater->appendChildHandler(sys);
		}

		return sys;
	}

	bool GameScene::hasSystem( const std::string& systemName ) const
	{
		return StdMap::containsKey(mImpl->compSystems, systemName);
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

		if (obj == mImpl->rootObject && status() != UpdateHandler::DESTROYING)
			cerr << "Destroy rootObject before scene's destroying" << endl;
	}

	UserEventReceiverStack& GameScene::getControllerStack()
	{
		return *mImpl->receiverStack;
	}

	void GameScene::onDestroy()
	{
		// Destroy everything that may use lua_State before CompositeUpdateHandler::onDestroy()
		mImpl->receiverStack->drop();
		mImpl->receiverStack = 0;

		CompositeUpdateHandler::onDestroy();
	}

}}

