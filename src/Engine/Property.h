#pragma once
#include <boost/variant/variant.hpp>
#include <string>
#include "vector3d.h"
#include "quaternion.h"
#include "LuaT/lua_ref.h"
#include "dimension2d.h"

namespace xihad { namespace ngn
{
	typedef boost::variant<
		bool, int, float, dimension2df, vector3df, quaternion,
		std::string, luaT::LuaRef
	> Property;	// property field can set to these type

}}

