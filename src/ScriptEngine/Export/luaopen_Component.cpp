#include <luaT/luaT.h>
#include "Engine/Component.h"
#include "../PushComponentVisitor.h"

using namespace luaT;
using namespace xihad::ngn;
namespace xihad { namespace script
{
	static int findPeer(lua_State* L) 
	{
		auto comp = checkarg<Component*>(L, 1);
		auto tname = checkarg<std::string&>(L, 2);

		Component* got = comp->findPeer(tname);
		if (got == nullptr || !got->accept(PushComponentVisitor(L)))
			lua_pushnil(L);

		return 1;
	}

	int luaopen_Component( lua_State* L )
	{
		luaT_defRegsBgn(componentRegs)
			luaT_mnamedfunc(Component, getHostObject),
			luaT_lnamedfunc(findPeer),
		luaT_defRegsEnd
		MetatableFactory<Component, UpdateHandler>::create(L, componentRegs, 0);

		return 0;
	}

}}
