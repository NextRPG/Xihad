#include "LuaManagedUpdateHandler.h"
#include "LuaUtil.h"
#include "luaT\stack_memo.h"

using namespace xihad::ngn;
using namespace luaT;
namespace xihad { namespace script
{
	LuaManagedUpdateHandler::LuaManagedUpdateHandler(LuaRef& table) :
		mLuaObjectRef(table)
	{
		lua_State* L = table.getState();
		StackMemo memo(L);
		
		table.pushSelf();
		xassert(lua_istable(L, -1));
		lua_pushlightuserdata(L, static_cast<ManagedUpdateHandler*>(this));
		lua_setfield(L, -2, luaT::ReservedKeyword::__UDKEY);
		Metatable::bind<ManagedUpdateHandler>(L, -1);
	}

	void LuaManagedUpdateHandler::onStart()
	{
		LuaUtil::callSelfErr(mLuaObjectRef, "onStart");
	}

	void LuaManagedUpdateHandler::onUpdate( const Timeline& )
	{
		LuaUtil::callSelfErr(mLuaObjectRef, "onUpdate");
	}

	void LuaManagedUpdateHandler::onStop()
	{
		LuaUtil::callSelfErr(mLuaObjectRef, "onStop");
	}

}}

