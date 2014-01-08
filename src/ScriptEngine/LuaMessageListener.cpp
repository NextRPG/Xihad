#include "LuaMessageListener.h"
#include "luaT/luaT.h"
#include "Message/MessageParam.h"
#include "Message/MessageTag.h"
#include "LuaUtil.h"

using namespace luaT;
using namespace xihad::ngn;
namespace xihad { namespace script
{

	LuaMessageListener::LuaMessageListener( luaT::LuaRef& obj ) :
		mObject(obj)
	{
		lua_State* L = mObject.getState();
		StackMemo memo(L);

		mObject.pushSelf();
		xassert(lua_istable(L, -1));

		lua_pushlightuserdata(L, static_cast<MessageListener*>(this));
		lua_setfield(L, -2, luaT::ReservedKeyword::__UDKEY);
		Metatable::bind<MessageListener>(L, -1);
	}

	void LuaMessageListener::receive( GameObject& pSource, const Parameter& pEvent )
	{
		lua_State* L = mObject.getState();
		StackMemo memo(L);

		mObject.pushSelf();
		luaT::getField(L, -1, "onMessage");

		mObject.pushSelf();
		push<GameObject*>(L, &pSource);
		pEvent.getLuaObject().pushOnto(L);
		push<const char*>(L, pEvent.getTag().toString().c_str());

		if (lua_pcall(L, 4, 0, 0) != 0)
			LuaUtil::outputErrorMessage(L, "LuaMessageListener::onMessage");
	}

	LuaMessageListener::~LuaMessageListener()
	{
	}

	void LuaMessageListener::onDestroy()
	{
		lua_State* L = mObject.getState();
		StackMemo memo(L);

		mObject.pushSelf();
		luaT::getField(L, -1, "onDestroy");

		if (lua_pcall(L, 0, 0, 0) != 0)
			LuaUtil::outputErrorMessage(L, "LuaMessageListener::onMessage");
	}

}}

