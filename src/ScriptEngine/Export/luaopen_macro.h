#pragma once

struct lua_State;

#define SCRIPT_LUAOPEN(module) \
	namespace xihad { namespace script		\
	{										\
		int luaopen_##module(lua_State* L);	\
	}}


