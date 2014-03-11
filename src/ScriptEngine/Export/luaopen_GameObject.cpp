#include <Engine\GameObject.h>
#include <Engine\Component.h>
#include <Engine\LuaProperties.h>
#include <luaT\luaT.h>
#include "..\PushComponentVisitor.h"


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

	static int object_getID(lua_State* L)
	{
		GameObject* go = checkarg<GameObject*>(L, 1);
		lua_pushstring(L, go->getID().c_str());
		return 1;
	}

	int luaopen_GameObject(lua_State* L)
	{
		luaT_defRegsBgn(goRegs)
			luaT_lnnamefunc(objectAppendComponent, appendComponent),
			luaT_lnnamefunc(objectFindComponent, searchComponent),
			luaT_lnnamefunc(objectFindComponent, findComponent),
			luaT_lnnamefunc(object_getID, getID),
			luaT_mnamedfunc(GameObject, setParent),
			luaT_mnamedfunc(GameObject, getParent),
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
		luaT_defRegsEnd
		MetatableFactory<GameObject, CompositeUpdateHandler>::create(L, goRegs, 0);

		return 0;
	}
}}

