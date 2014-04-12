#pragma once
#include "CEGUIHandlerDef.h"

namespace xihad { namespace ngn
{
	class IrrlichtWindow;
}}
struct lua_State;
namespace xihad { namespace cegui
{
	class CEGUIUpdateHandler;
	CEGUI_HANDLER_API CEGUIUpdateHandler* createGUI(ngn::IrrlichtWindow* irrWnd, lua_State* L);
}}

