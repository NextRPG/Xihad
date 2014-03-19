#pragma once

struct lua_State;

namespace irr 
{
	class IrrlichtDevice;
}

namespace xihad { namespace render3d
{
	int luaopen_Cursor(lua_State* L);
	int luaopen_render3dComponents(lua_State* L);
	int luaopen_SceneCollisionManager(lua_State* L);
	int luaopen_Geometry(lua_State* L);
	int luaopen_RenderSystem(lua_State* L);
	int luaopen_ResourceManager(lua_State* L);
	int luaopen_Material(lua_State* L);
	int luaopen_SColor(lua_State* L);

	class IrrlichtComponentSystem;
	void luaopen_All(irr::IrrlichtDevice* dev, IrrlichtComponentSystem*, lua_State* L);
}}

