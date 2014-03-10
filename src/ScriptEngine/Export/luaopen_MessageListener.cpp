#include <LuaT\luaT.h>
#include <Engine\Message\MessageListener.h>
#include <Engine\xptr.h>

using namespace luaT;
using namespace xihad::ngn;

luaT_defMetaData(MessageListener, true);

namespace xihad { namespace script
{
	int luaopen_MessageListener( lua_State* L )
	{
		luaT_defRegsBgn(msgLister)
			luaT_cnnamefunc(intrusive_ptr_add_ref<MessageListener>, grab),
			luaT_cnnamefunc(intrusive_ptr_release<MessageListener>, drop),
		luaT_defRegsEnd
		MetatableFactory<MessageListener>::create(L, msgLister);
		return 0;
	}
}}

