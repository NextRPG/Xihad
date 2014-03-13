#pragma once

struct lua_State;
namespace xihad { namespace script
{
	int luaopen_string(lua_State* L);
	int luaopen_UpdateHandler(lua_State* L);
	int luaopen_CompositeUpdateHandler(lua_State* L);
	int luaopen_Component(lua_State* L);
	int luaopen_LuaComponent(lua_State* L);
	int luaopen_GameScene(lua_State* L);
	int luaopen_GameObject(lua_State* L);
	int luaopen_MessageDispatcher(lua_State* L);
	int luaopen_MessageListener(lua_State* L);
	int luaopen_NativeWindow(lua_State* L);
	int luaopen_GameEngine(lua_State* L);
	int luaopen_GameWorld(lua_State* L);
	
	void luaopen_all(lua_State* L)
	{
		luaT::StackMemo memo(L);
		script::luaopen_string(L);
		luaopen_UpdateHandler(L);
		luaopen_CompositeUpdateHandler(L);
		luaopen_Component(L);
		luaopen_LuaComponent(L);
		luaopen_GameScene(L);
		luaopen_GameObject(L);
		luaopen_MessageDispatcher(L);
		luaopen_MessageListener(L);
		luaopen_NativeWindow(L);
		luaopen_GameEngine(L);
		luaopen_GameWorld(L);
	}
}}

