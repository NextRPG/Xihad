#include "MessageParam.h"
#include "luaT\lua_ref.h"
#include "MessageTag.h"

namespace xihad { namespace ngn 
{
	struct MessageParam::impl
	{
		impl(const MessageTag& pTag, const luaT::LuaRef& pRef)
			: tag(pTag), ref(pRef) {}

		luaT::LuaRef ref;
		MessageTag tag;
	};

	MessageParam::MessageParam( const MessageTag& pTag, const luaT::LuaRef& pRef )
		: mParam(new impl(pTag, pRef))
	{
	}

	const MessageTag& MessageParam::getTag() const
	{
		return mParam->tag;
	}

	luaT::LuaRef MessageParam::getLuaObject() const
	{
		return mParam->ref;
	}

}}
