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
		luaT_defRegsBgn(msgLister)
			luaT_cnnamefunc(intrusive_ptr_add_ref, grab),
			luaT_cnnamefunc(intrusive_ptr_release, drop),
		luaT_defRegsEnd
		MetatableFactory<MessageListener>::create(L, msgLister);
		return 0;
	}
}}

