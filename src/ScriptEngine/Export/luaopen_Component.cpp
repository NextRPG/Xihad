#include "luaopen_Component.h"
#include <luaT/luaT.h>
#include "Engine/Component.h"

using namespace luaT;
using namespace xihad::ngn;

luaT_defMetaData(Component, true)

static int appendDestroyable(lua_State* L)
{
	Component* c	= checkarg<Component*>(L, 1);
	Destroyable* d	= checkarg<Destroyable*>(L, 2);
	c->appendDestroyable(d);
	return 0;
}

namespace xihad { namespace script
{
	int luaopen_Component( lua_State* L )
	{
		luaT_defRegsBgn(componentRegs)
			luaT_mnamedfunc(Component, getHostObject),
			// luaT_mnamedfunc(Component, appendDestroyable),
			luaT_lnamedfunc(appendDestroyable),
			luaT_mnamedfunc(Component, removeDestroyable),
		luaT_defRegsEnd
		MetatableFactory<Component, ManagedUpdateHandler>::create(L, componentRegs, 0);

		return 0;
	}

}}
