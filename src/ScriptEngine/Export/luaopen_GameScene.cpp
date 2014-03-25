#include <luaT/luaT.h>
#include <CppBase/IteratorPair.h>
#include <Engine/GameScene.h>
#include <Engine/UserEventReceiverStack.h>
#include "../LuaEventReceiver.h"

using namespace luaT;
using namespace xihad;
using namespace ngn;
luaT_defMetaData(GameScene, false);
luaT_defMetaData(UserEventReceiver, false);

namespace xihad { namespace script
{
	typedef IteratorPair<const GameScene::ObjectGroup> ObjectIterPair;

	// list, var
	static int objectIterator(lua_State* L)
	{
		xassert(lua_islightuserdata(L, 1));
		ObjectIterPair* iterPtr = 
			static_cast<ObjectIterPair*> (lua_touserdata(L, 1));

		if (iterPtr != nullptr && iterPtr->notEnd())
		{
			ObjectIterPair& iter = *iterPtr;
			push<GameObject*>(L, *iter);
			iter.next();
		}
		else
		{
			lua_pushnil(L);
			delete iterPtr;
		}

		return 1;
	}

	static int eachObjectWithTag(lua_State* L)
	{
		GameScene* scene = checkarg<GameScene*>(L, 1);
		const char* tag = checkarg<const char*>(L, 2);

		// Get list
		auto objList = scene->findObjectsByTag(tag);

		// Create Iterator Pair
		void* iterData = nullptr;
		if (objList)
			iterData = new ObjectIterPair(*objList);

		lua_pushcfunction(L, objectIterator);
		lua_pushlightuserdata(L, iterData);
		lua_pushnil(L);
		return 3;
	}

	static int sceneCreateObject(lua_State* L)
	{
		luaL_checkany(L, 2);
		GameScene* scene = checkarg<GameScene*>(L, 1);
		std::string& name = checkarg<std::string&>(L, 2);

		GameObject* obj;
		int n = lua_gettop(L);
		if (n == 2)
			obj = scene->createObject(name);
		else
			obj = scene->createObject(name, checkarg<GameObject*>(L, 3));

		push<GameObject*>(L, obj);
		return 1;
	}
	
	static int sceneCreateUniqueObject(lua_State* L)
	{
		GameScene* scene = checkarg<GameScene*>(L, 1);
		std::string& header = checkarg<std::string&>(L, 2);

		GameObject* parent = NULL;
		bool addToTag = true;
		int n = lua_gettop(L);
		if (n >= 3)
		{
			parent = checkarg<GameObject*>(L, 3);
			if (n == 4)
				addToTag = checkarg<bool>(L, 4);
		}

		GameObject* obj = scene->createUniqueObject(header, parent, addToTag);

		push<GameObject*>(L, obj);
		return 1;
	}
	static int scenePushController(lua_State* L)
	{
		luaL_checkany(L, 2);
		GameScene* scene = checkarg<GameScene*>(L, 1);

		UserEventReceiver* receiver;
		if ((receiver = checkarg<UserEventReceiver*>(L, 2)) == 0)
		{
			LuaRef tref = LuaRef::fromIndex(L, 2);
			receiver = new LuaEventReceiver(tref);
		}
		
		scene->getControllerStack().pushReceiver(receiver);
		return 0;
	}

	luaT_static UserEventReceiver* scenePopController(GameScene* scene)
	{
		return scene->getControllerStack().popReceiver();
	}}

	luaT_static bool drop(UserEventReceiver* rec)
	{
		return rec->drop();
	}}

	int luaopen_GameScene( lua_State* L )
	{
		luaT_defRegsBgn(receiver)
			luaT_cnamedfunc(drop),
		luaT_defRegsEnd
		MetatableFactory<UserEventReceiver>::create(L, receiver);

		luaT_defRegsBgn(sceneRegs)
			{ "objectsWithTag", eachObjectWithTag },
			luaT_mnamedfunc(GameScene, findObject), 
			luaT_mnamedfunc(GameScene, getDispatcher),
			{ "createObject", sceneCreateObject },
			{ "createUniqueObject", sceneCreateUniqueObject },
			{ "pushController", scenePushController },
			{ "popController", luaT_cfunction(scenePopController) },
		luaT_defRegsEnd
		MetatableFactory<GameScene, CompositeUpdateHandler>::create(L, sceneRegs, 0);

		return 0;
	}

}}

