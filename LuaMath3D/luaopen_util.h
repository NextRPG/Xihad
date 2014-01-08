#pragma once
#include <LuaT/luaT.h>

// 导出二元操作符
#define BI_OPERATOR(Type, Signal) \
	luaT_mfunction_ovl(\
	Type (Type::*)(const Type&) const, \
	Type::operator Signal\
	)

// 导出一元操作符
#define UN_OPERATOR(Type, Signal) \
	luaT_mfunction_ovl(\
	Type (Type::*)() const, \
	Type::operator Signal\
	)


