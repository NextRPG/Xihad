#include <luaT/luaT.h>
#include <Engine/GameScene.h>
#include <Engine/Message/MessageTag.h>
#include "../LuaMessageListener.h"

using namespace luaT;
using namespace xihad::ngn;
typedef GameScene::Dispatcher Dispatcher;

luaT_defMetaData(Dispatcher, false);

namespace xihad { namespace script
{
	// Dispatcher::dispatch(tagString, any, sourceObjID[, timeout])
	static int dispatch(lua_State* L)
	{
		Dispatcher* dsptch = checkarg<Dispatcher*>(L, 1);
		const char* tagString = checkarg<const char*>(L, 2);
		Dispatcher::Parameter param(std::string(tagString), LuaRef::fromIndex(L, 3));
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

	static int addListener(lua_State* L)
	{
		Dispatcher* dsptch = checkarg<Dispatcher*>(L, 1);
		const char* tag = checkarg<const char*>(L, 2);

		MessageListener* lis;
		if ((lis = checkarg<MessageListener*>(L, 3)) == 0)
		{
			luaL_checktype(L, 3, LUA_TTABLE);
			lis = new LuaMessageListener(LuaRef::fromIndex(L, 3));
		}
		
		dsptch->addListener(std::string(tag), lis);
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

