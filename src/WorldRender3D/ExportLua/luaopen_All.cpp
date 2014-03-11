#include "luaopen_All.h"
#include <LuaT\table_ops.h>
#include <luaT\stack_memo.h>
#include <irrlicht\IrrlichtDevice.h>
#include <irrlicht\ICursorControl.h>
#include <irrlicht\ISceneManager.h>
#include "LuaGlobalVariable.h"
#include "..\IrrlichtComponentSystem.h"
#include "..\Geometry.h"

namespace xihad { namespace render3d
{
	int luaopen_Cursor(lua_State* L);
	int luaopen_render3dComponents(lua_State* L);
	int luaopen_SceneCollisionManager(lua_State* L);
	int luaopen_Geometry(lua_State* L);
	int luaopen_RenderSystem(lua_State* L);
	int luaopen_ResourceManager(lua_State* L);

	using namespace irr;
	using namespace luaT;
	void luaopen_All(IrrlichtDevice* dev, IrrlichtComponentSystem* ics, lua_State* L)
	{
		luaT::StackMemo memo(L);

		luaopen_Cursor(L);
		luaopen_render3dComponents(L);
		luaopen_SceneCollisionManager(L);
		luaopen_Geometry(L);
		luaopen_RenderSystem(L);
		luaopen_ResourceManager(L);

		lua_getglobal(L, "_G");
		gui::ICursorControl* cursor = dev->getCursorControl();
		setField(L, -1, LUA_G_CURSOR, cursor);
		setField(L, -1, LUA_G_GEOMETRY, Geometry::creator());

		scene::ISceneCollisionManager* collMan = ics->getSceneManager()->getSceneCollisionManager();
		setField(L, -1, LUA_G_COLLISION, collMan);

		setField(L, -1, LUA_G_TEX_MANAGER, ics->getTextureManager());
		setField(L, -1, LUA_G_MESH_MANAGER, ics->getMeshManager());
	}	
}}
