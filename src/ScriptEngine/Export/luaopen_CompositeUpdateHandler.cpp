#include "luaopen_CompositeUpdateHandler.h"
#include "luaopen_ManagedUpdateHandler.h"
#include <luaT/luaT.h>
#include "Engine/CompositeUpdateHandler.h"
#include "ScriptEngine/LuaUtil.h"
#include "../LuaManagedUpdateHandler.h"

using namespace luaT;
using namespace xihad::ngn;

luaT_defMetaData(CompositeUpdateHandler, true);

namespace xihad { namespace script
{
	static int appendUpdater(lua_State* L)
	{
		luaL_checkany(L, 2);
		auto group = checkarg<CompositeUpdateHandler*>(L, 1);

		ManagedUpdateHandler* updater;
		if (lua_istable(L, 2))
		{
			updater = new LuaManagedUpdateHandler(LuaRef::fromIndex(L, 2));
		}
		else if ((updater = checkarg<ManagedUpdateHandler*>(L, 2)) == NULL)
		{
			luaL_typerror(L, 2, "appendUpdater requires table/UpdateHandler");
		}

		push<bool>(L, group->appendUpdateHandler(updater));
		return 1;
	}

	luaT_static bool removeUpdaterImpl(CompositeUpdateHandler* group, 
		ManagedUpdateHandler* updater)
	{
		return group->removeUpdateHandler(updater);
	}}

	int luaopen_CompositeUpdateHandler(lua_State* L)
	{
		auto removeUpdater = luaT_cfunction(removeUpdaterImpl);

		luaT_defRegsBgn(cuhRegs)
			{ "appendUpdater", appendUpdater },
			{ "removeUpdater", removeUpdater},
			{ "appendUpdateHandler", appendUpdater },
			{ "removeUpdateUpdater", removeUpdater},
		luaT_defRegsEnd
		MetatableFactory<CompositeUpdateHandler, 
			ManagedUpdateHandler>::create(L, cuhRegs, 0);

		return 0;
	}
}}

