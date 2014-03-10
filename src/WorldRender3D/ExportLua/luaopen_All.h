#pragma once

struct lua_State;

namespace irr 
{
	class IrrlichtDevice;

	namespace scene
	{
		class ISceneManager;
	}
}

namespace xihad { namespace render3d
{
	int luaopen_Cursor(lua_State* L);
	int luaopen_render3dComponents(lua_State* L);
	int luaopen_SceneCollisionManager(lua_State* L);
	int luaopen_Geometry(lua_State* L);
	int luaopen_RenderSystem(lua_State* L);

	void luaopen_All(irr::IrrlichtDevice* dev, irr::scene::ISceneManager* smgr, lua_State* L);
}}

