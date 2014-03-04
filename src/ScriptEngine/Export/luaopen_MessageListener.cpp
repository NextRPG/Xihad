#include "luaopen_MessageListener.h"
#include "LuaT\luaT.h"
#include "Message\MessageListener.h"

using namespace luaT;
using namespace xihad::ngn;

luaT_defMetaData(MessageListener, true);

namespace xihad { namespace script
{
	int luaopen_MessageListener( lua_State* L )
	{
		MetatableFactory<MessageListener>::create(L, 0);
		return 0;
	}
}}

