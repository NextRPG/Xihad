#include "LuaComponent.h"
#include "LuaT/luaT.h"
#include "LuaUtil.h"
#include "luaT/stack_memo.h"
#include <iostream>
#include "LuaScriptMacro.h"

using namespace xihad::ngn;
using namespace std;
using namespace luaT;
namespace xihad { namespace script
{
	LuaComponent::LuaComponent( 
		const string& rttiName, GameObject& host, LuaRef& luaObject ) :
		Component(rttiName, host), mLuaObjectRef(luaObject)
	{
		lua_State* L = mLuaObjectRef.getState();
		StackMemo memo(L);

		mLuaObjectRef.pushSelf();
		xassert(lua_istable(L, -1));

		lua_pushlightuserdata(L, this);
		lua_setfield(L, -2, luaT::ReservedKeyword::__UDKEY);

		push<GameObject*>(L, &host);
		lua_setfield(L, -2, LUA_COMPONENT_OBJECT);
	}

	void LuaComponent::onStart()
	{
		LuaUtil::callSelfErr(mLuaObjectRef, "onStart");
	}

	void LuaComponent::onUpdate( const Timeline& time )
	{
		// time have been set via LuaComponentSystem
		LuaUtil::callSelfErr(mLuaObjectRef, "onUpdate");
	}

	void LuaComponent::onStop()
	{
		LuaUtil::callSelfErr(mLuaObjectRef, "onStop");
	}

	LuaComponent::~LuaComponent()
	{
	}

}}

