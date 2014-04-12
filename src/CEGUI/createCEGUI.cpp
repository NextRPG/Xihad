#include "CEGUIUpdataHandler.h"
#include "Engine\IrrlichtWindow.h"
#include "CEGuiHandle.h"
#include "CEGUIHandlerDef.h"

namespace xihad { namespace cegui
{
	CEGUI_HANDLER_API CEGUIUpdateHandler* createGUI(ngn::IrrlichtWindow* irrWnd, lua_State* L)
	{
		CEGUIHandle* handle = new CEGUIHandle;
		irr::IrrlichtDevice* device = irrWnd->getIrrDevice();
		handle->initialise(device, L);

		return new CEGUIUpdateHandler(handle);
	}
}}
