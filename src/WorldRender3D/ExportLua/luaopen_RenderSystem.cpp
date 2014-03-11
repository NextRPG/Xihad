#include <irrlicht/ISceneManager.h>
#include <luaT/luaT.h>

using namespace irr::scene;
using namespace luaT;
namespace xihad { namespace render3d
{
	int luaopen_RenderSystem(lua_State* L)
	{
		/// SceneManager
		// Cache mesh
		// Get mesh
		// shadow color
		// ambient color
// 		// fog

		/// VideoDriver
		// texture management: getTexture/renameTexture
		// render target
		// viewport
		luaT_defRegsBgn(rsRegs)

		luaT_defRegsEnd

		return 0;
	}
}}

