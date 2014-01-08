#include "luaopen_renderSystem.h"
#include <irrlicht/ISceneManager.h>
#include <luaT/luaT.h>

using namespace irr::scene;
using namespace luaT;
namespace xihad { namespace script
{
	int luaopen_RenderSystem(lua_State* L)
	{
		// vd fog
		// vd texture management
		// vd image r/w
		// vd render target
		// vd viewport
		// vd primitive draw
		// vd clipplane
		// vd material mangement
		luaT_defRegsBgn(rsRegs)
			// smgr shadow color
			// smgr ambient color
			// luaT_mnamedfunc(ISceneManager, )
		luaT_defRegsEnd

		return 0;
	}
}}

