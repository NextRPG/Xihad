#include "luaopen_MessageListener.h"
#include "luaopen_Destroyable.h"
#include "LuaT\luaT.h"
#include "Message\MessageListener.h"

using namespace luaT;
using namespace xihad::ngn;

luaT_defMetaData(MessageListener, true);

namespace xihad { namespace script
{
	int luaopen_MessageListener( lua_State* L )
	{
		luaT_defRegsBgn(regs)
		luaT_defRegsEnd
		MetatableFactory<MessageListener, Destroyable>::create(L, regs, 0);
		return 0;
	}
}}

