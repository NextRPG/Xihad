#include "LuaUpdateHandler.h"
#include <Engine/MemoryLeakDetector.h>
#include "LuaUtil.h"
#include "luaT/stack_memo.h"

using namespace xihad::ngn;
using namespace luaT;
namespace xihad { namespace script
{
	LuaUpdateHandler::LuaUpdateHandler(LuaRef& table) : mLuaObjectRef(table)
	{
		lua_State* L = table.getState();
		StackMemo memo(L);
		
		table.pushSelf();
		xassert(lua_istable(L, -1));
		lua_pushlightuserdata(L, static_cast<UpdateHandler*>(this));
		lua_setfield(L, -2, luaT::ReservedKeyword::__UDKEY);
		Metatable::bind<UpdateHandler>(L, -1);

		XIHAD_MLD_NEW_OBJECT;
	}

	void LuaUpdateHandler::onStart()
	{
		LuaUtil::callSelfErr(mLuaObjectRef, "onStart");
	}

	void LuaUpdateHandler::onUpdate( const Timeline& )
	{
		LuaUtil::callSelfErr(mLuaObjectRef, "onUpdate");
	}

	void LuaUpdateHandler::onStop()
	{
		LuaUtil::callSelfErr(mLuaObjectRef, "onStop");
	}

	LuaUpdateHandler::~LuaUpdateHandler()
	{
		XIHAD_MLD_DEL_OBJECT;
	}

}}

