#include "LuaEventReceiver.h"
#include <iostream>
#include "luaT\stack_memo.h"
#include "LuaUtil.h"
#include "LuaT\table_ops.h"
#include "Engine\UserEvent.h"
#include <iostream>

using namespace xihad::ngn;
using namespace luaT;
using namespace std;
namespace xihad { namespace script
{
	LuaEventReceiver::LuaEventReceiver( const luaT::LuaRef& object ) :
		lobject(object)
	{
		setDebugName("LuaEventReceiver");
		XIHAD_MLD_NEW_OBJECT;

		StackMemo memo(lobject.getState());
		lobject.pushSelf();
		luaL_checktype(lobject.getState(), -1, LUA_TTABLE);
	}

	LuaEventReceiver::~LuaEventReceiver()
	{
		XIHAD_MLD_DEL_OBJECT;
	}

	static void pushParam(lua_State* L, const MouseEvent& e)
	{
		lua_createtable(L, 0, 7);
		static const char* eventTable[irr::EMOUSE_INPUT_EVENT::EMIE_COUNT] =
		{
			"lPressed", "rPressed", "mPressed", 
			"lUplift",	"rUplift",	"mUplift",
			"mouseMoved",  "wheelMoved",
			"lDoubleClick", "rDoubleClick", "mDoubleClick",
			"lTripleClick", "rTripleClick", "mTripleClick",
		};

		const char* type;
		if (e.Event < 0 || e.Event >= irr::EMOUSE_INPUT_EVENT::EMIE_COUNT)
			type = "unknown";
		else
			type = eventTable[e.Event];

		setField(L, -1, "type", type);							// string
		setField(L, -1, "wheelDelta", e.Wheel);					// number
		setField(L, -1, "shiftPressed", e.Shift);				// bool
		setField(L, -1, "controlPressed", e.Control);			// bool
		setField(L, -1, "leftPressed", e.isLeftPressed());		// bool
		setField(L, -1, "rightPressed", e.isRightPressed());	// bool
		setField(L, -1, "middlePressed", e.isMiddlePressed());	// bool
	}

	static void pushParam(lua_State* L, const KeyEvent& e)
	{
		lua_createtable(L, 0, 4);
		setField(L, -1, "key", KeyCode_helper::toString(e.Key));	// string
		setField(L, -1, "shift",	e.Shift);		// bool
		setField(L, -1, "control",	e.Control);	// bool
	}

	template <class EventParam>
	int callWithParam(luaT::LuaRef& lobject, const char* func, const EventParam& param, int arg)
	{
		lua_State* L = lobject.getState();
		StackMemo memo(L);

		lobject.pushSelf();
		lua_getfield(L, -1, func);

		if (lua_isnil(L, -1)) return false;

		lobject.pushSelf();

		pushParam(L, param);
		lua_pushinteger(L, arg);
		if (lua_pcall(L, 3, 1, 0))
		{
			LuaUtil::outputErrorMessage(L, func);
			return false;
		}

		if (!lua_isnumber(L, -1))
		{
			cout << "WARNING: EXPLICITLY return integer in event handler" << endl;
			return 0;
		}

		return lua_tointeger(L, -1);
	}

	int LuaEventReceiver::onKeyEvent( const KeyEvent& event, int argFromPreviousReceiver )
	{
		if (event.PressedDown)
			return callWithParam(lobject, "onKeyDown", event, argFromPreviousReceiver);
		else
			return callWithParam(lobject, "onKeyUp", event, argFromPreviousReceiver);
	}

	int LuaEventReceiver::onMouseEvent( const MouseEvent& event, int argFromPreviousReceiver )
	{
		return callWithParam(lobject, "onMouseEvent", event, argFromPreviousReceiver);
	}

}}

