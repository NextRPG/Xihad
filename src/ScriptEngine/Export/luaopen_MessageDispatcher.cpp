#include "luaT/luaT.h"
#include "Engine/GameScene.h"
#include "Message/MessageTag.h"
#include "../LuaMessageListener.h"

using namespace luaT;
using namespace xihad::ngn;
typedef GameScene::Dispatcher Dispatcher;

luaT_defMetaData(Dispatcher, false);

namespace xihad { namespace script
{
	// Dispatcher::dispatch(tagString, any, sourceObjID[, timeout])
	int dispatch(lua_State* L)
	{
		Dispatcher* dsptch = checkarg<Dispatcher*>(L, 1);
		const std::string& tagString = checkarg<std::string&>(L, 2);
		Dispatcher::Parameter param(tagString, LuaRef::fromIndex(L, 3));
		const std::string& objID = checkarg<std::string&>(L, 4);

		if (lua_gettop(L) == 4)
			dsptch->dispatch(param, objID);
		else
		{
			double timeout = checkarg<double>(L, 5);
			dsptch->dispatch(param, objID, timeout);
		}

		return 0;
	}

	int addListener(lua_State* L)
	{
		Dispatcher* dsptch = checkarg<Dispatcher*>(L, 1);
		std::string& tag = checkarg<std::string&>(L, 2);
		luaL_checktype(L, 3, LUA_TTABLE);
		MessageListener* lis = new LuaMessageListener(LuaRef::fromIndex(L, 3));
		dsptch->addListener(std::string(tag), lis, nullptr);

		return 0;
	}

	luaT_static void removeListener(Dispatcher* disp, const char* tag, MessageListener* listener)
	{
		disp->removeListener(MessageTag(tag), listener);
	}}

	int luaopen_MessageDispatcher(lua_State* L)
	{
		luaT_defRegsBgn(regs)
			luaT_lnamedfunc(dispatch),
			luaT_lnamedfunc(addListener),
			luaT_mnamedfunc(Dispatcher, clearListener),
			luaT_cnamedfunc(removeListener),
		luaT_defRegsEnd
		MetatableFactory<Dispatcher>::create(L, regs, 0);
		return 0;
	}
}}

