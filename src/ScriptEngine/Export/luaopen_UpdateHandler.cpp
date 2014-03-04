#include "luaopen_UpdateHandler.h"
#include <luaT/luaT.h>
#include "Engine/UpdateHandler.h"
#include <typeinfo>

using namespace luaT;
using namespace xihad::ngn;
luaT_defMetaData(UpdateHandler, true);

namespace xihad { namespace script
{
	luaT_static const char* status(UpdateHandler* handler)
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

	int luaopen_UpdateHandler( lua_State* L )
	{
		luaT_defRegsBgn(updater)
			luaT_mnamedfunc(UpdateHandler, start),
			luaT_mnamedfunc(UpdateHandler, stop),
			luaT_mnamedfunc(UpdateHandler, destroy),
			luaT_cnamedfunc(status),
		luaT_defRegsEnd
		MetatableFactory<UpdateHandler>::create(L, updater, 0);

		return 0;
	}

}}
