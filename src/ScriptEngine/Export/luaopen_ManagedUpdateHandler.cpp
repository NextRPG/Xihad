#include "luaopen_ManagedUpdateHandler.h"
#include <luaT/luaT.h>
#include "Engine/ManagedUpdateHandler.h"
#include <typeinfo>

using namespace luaT;
using namespace xihad::ngn;
luaT_defMetaData(ManagedUpdateHandler, true);

namespace xihad { namespace script
{
	luaT_static const char* status(ManagedUpdateHandler* handler)
	{
		UpdateHandler::Status stt = handler->status();
		switch (stt)
		{
		case xihad::ngn::UpdateHandler::BORN:
			return "born";
		case xihad::ngn::UpdateHandler::STARTING:
			return "starting";
		case xihad::ngn::UpdateHandler::UPDATING:
			return "updating";
		case xihad::ngn::UpdateHandler::UPDATED:
			return "updated";
		case xihad::ngn::UpdateHandler::STOPPING:
			return "stopping";
		case xihad::ngn::UpdateHandler::DEAD:
			return "dead";
		case xihad::ngn::UpdateHandler::DESTROYING:
			return "destroying";
		default:
			return "error";
		}
	}}

	int luaopen_ManagedUpdateHandler( lua_State* L )
	{
		luaT_defRegsBgn(updater)
			luaT_mnamedfunc(ManagedUpdateHandler, start),
			luaT_mnamedfunc(ManagedUpdateHandler, stop),
			luaT_mnamedfunc(ManagedUpdateHandler, destroy),
			luaT_mnamedfunc(ManagedUpdateHandler, isDestroying),
			luaT_cnamedfunc(status),
		luaT_defRegsEnd
		MetatableFactory<ManagedUpdateHandler>::create(L, updater, 0);

		return 0;
	}

}}
