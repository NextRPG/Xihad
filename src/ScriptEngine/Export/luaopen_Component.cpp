#include "luaopen_Component.h"
#include <luaT/luaT.h>
#include "Engine/Component.h"

using namespace luaT;
using namespace xihad::ngn;

luaT_defMetaData(Component, true)

namespace xihad { namespace script
{
	int luaopen_Component( lua_State* L )
	{
		luaT_defRegsBgn(componentRegs)
			luaT_mnamedfunc(Component, getHostObject),
		luaT_defRegsEnd
		MetatableFactory<Component, UpdateHandler>::create(L, componentRegs, 0);

		return 0;
	}

}}
