#include "luaopen_GameObject.h"
#include "luaopen_CompositeUpdateHandler.h"
#include "Engine\GameObject.h"
#include <luaT\luaT.h>
#include "ScriptEngine\PushComponentVisitor.h"
#include "Engine\Component.h"
#include "Engine\LuaProperties.h"
#include "luaopen_string.h"

using namespace luaT;
using namespace xihad::ngn;

luaT_defMetaData(GameObject, false);

namespace xihad { namespace script
{
	static int objectAppendComponent(lua_State* L)
	{
		luaL_checkany(L, 2);
		auto obj = checkarg<GameObject*>(L, 1);
		auto& comp = checkarg<std::string&>(L, 2);

		std::pair<Component*, bool> result;
		if (lua_gettop(L) == 2 || lua_isnil(L, 3))
		{
			result = obj->appendComponent(comp);
		}
		else
		{
			LuaRef tableRef = LuaRef::fromIndex(L, 3);
			LuaProperties param(tableRef);
			result = obj->appendComponent(comp, param);
		}

		if (!result.first || !result.first->accept(PushComponentVisitor(L)))
			lua_pushnil(L);

		push<bool>(L, result.second);
		return 2;
	}

	static int objectFindComponent(lua_State* L)
	{
		luaL_checkany(L, 2);
		auto obj = checkarg<GameObject*>(L, 1);
		auto comp = checkarg<std::string&>(L, 2);

		Component* got = obj->findComponent(comp);
		if (got == nullptr || !got->accept(PushComponentVisitor(L)))
			lua_pushnil(L);

		return 1;
	}

	static int pushVec3(lua_State* L, const vector3df& r)
	{
		push<float>(L, r.X);
		push<float>(L, r.Y);
		push<float>(L, r.Z);

		return 3;
	}

	static int object_getRotation(lua_State* L)
	{
		GameObject* go = checkarg<GameObject*>(L, 1);
		return pushVec3(L, go->getRotation());
	}

	static int object_getTranslate(lua_State* L)
	{
		GameObject* go = checkarg<GameObject*>(L, 1);
		return pushVec3(L, go->getTranslate());
	}

	static int object_getScale(lua_State* L)
	{
		GameObject* go = checkarg<GameObject*>(L, 1);
		return pushVec3(L, go->getScale());
	}

	static int object_getID(lua_State* L)
	{
		GameObject* go = checkarg<GameObject*>(L, 1);
		lua_pushstring(L, go->getID().c_str());
		return 1;
	}

	int luaopen_GameObject(lua_State* L)
	{
		luaT_defRegsBgn(goRegs)
			{ "appendComponent", objectAppendComponent },
			{ "searchComponent", objectFindComponent },
			{ "findComponent",	 objectFindComponent },
			luaT_mnamedfunc(GameObject, removeComponent),
			luaT_mnamedfunc(GameObject, getScene),
			luaT_mnamedfunc(GameObject, addTag),
			luaT_mnamedfunc(GameObject, removeTag),
			luaT_mnamedfunc(GameObject, hasTag),
			luaT_mnamedfunc(GameObject, concatScale),
			luaT_mnamedfunc(GameObject, concatTranslate),
			luaT_mnamedfunc(GameObject, concatRotate),
			luaT_mnamedfunc(GameObject, concatUniScale),
			luaT_mnamedfunc(GameObject, resetRotate),
			luaT_mnamedfunc(GameObject, resetScale),
			luaT_mnamedfunc(GameObject, resetTranslate),
			luaT_mnnamefunc(GameObject, resetTranslate, resetTranslation),
			luaT_mnnamefunc(GameObject, resetRotate,	resetRotation),
			luaT_mnamedfunc(GameObject, getScale),
			luaT_mnamedfunc(GameObject, getTranslate),
			luaT_mnnamefunc(GameObject, getTranslate, getTranslation),
			luaT_mnamedfunc(GameObject, getRotation),
			luaT_mnnamefunc(GameObject, getRotation, getRotate),
			{ "getID",			object_getID },
		luaT_defRegsEnd
		MetatableFactory<GameObject, CompositeUpdateHandler>::create(L, goRegs, 0);

		return 0;
	}
}}

