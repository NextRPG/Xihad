#include "luaopen_CompositeUpdateHandler.h"
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

		UpdateHandler* updater;
		if (lua_istable(L, 2))
		{
			updater = new LuaManagedUpdateHandler(LuaRef::fromIndex(L, 2));
		}
		else if ((updater = checkarg<UpdateHandler*>(L, 2)) == NULL)
		{
			luaL_typerror(L, 2, "appendUpdater requires table/UpdateHandler");
		}

		push<bool>(L, group->appendChildHandler(updater));
		return 1;
	}

	luaT_static bool removeUpdaterImpl(CompositeUpdateHandler* group, 
		UpdateHandler* updater)
	{
		auto it = group->findChildHandler(updater);
		if (it == group->childHandlerEnd())
			return false;

		group->eraseChildHandler(it);
		return true;
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
		MetatableFactory<CompositeUpdateHandler>::create(L, cuhRegs, 0);

		return 0;
	}
}}

