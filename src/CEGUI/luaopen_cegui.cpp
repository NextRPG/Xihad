#include <luaT/luaT.h>
#include "Engine/IrrlichtWindow.h"
#include "CEGuiHandle.h"
#include "CEGUIUpdataHandler.h"

using namespace xihad;
using namespace ngn;
using namespace cegui;
static int createGUI(lua_State* L)
{
	luaT_variable(L, 1, NativeWindow*, wnd);
	if (IrrlichtWindow* irrWnd = dynamic_cast<IrrlichtWindow*>(wnd))
	{
		CEGUIHandle* handle = new CEGUIHandle;
		irr::IrrlichtDevice* device = irrWnd->getIrrDevice();
		handle->initialise(device, L);

		luaT::push<CEGUIUpdateHandler*>(L, new CEGUIUpdateHandler(handle));
		return 1;
	}
	else
	{
		luaL_typerror(L, 1, "This dll only support for IrrlichtWindow");
		return 0;
	}
}

extern "C" __declspec(dllexport) int luaopen_cegui(lua_State* L)
{
	luaT_defRegsBgn(updater)
		luaT_mnamedfunc(CEGUIUpdateHandler, getEventReceiver),
	luaT_defRegsEnd
	luaT::MetatableFactory<CEGUIUpdateHandler, UpdateHandler>::create(L, updater, 0);

	lua_pushcfunction(L, createGUI);
	lua_setglobal(L, "createCEGUIUpdateHandler");

	return 1;
}

