#pragma once
#include "boost/shared_ptr.hpp"
#include "luaT/lua_ref.h"

namespace luaT { class LuaRef; }
namespace xihad { namespace ngn 
{
	class MessageTag;
	class MessageParam
	{
	public:
		MessageParam(const MessageTag& pTag, const luaT::LuaRef& pRef);
		
		const MessageTag& getTag() const;

		luaT::LuaRef getLuaObject() const;

	private:
		struct impl;
		boost::shared_ptr<impl> mParam;
	};
}}


