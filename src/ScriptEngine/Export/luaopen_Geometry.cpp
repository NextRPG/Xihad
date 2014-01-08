#include "luaopen_Geometry.h"
#include <luaT/luaT.h>
#include <irrlicht/IGeometryCreator.h>
#include "Engine/SColor.h"
#include "irrlicht/IMesh.h"
#include "irrlicht/IMeshBuffer.h"

using namespace xihad;
using namespace luaT;
using namespace irr;
using namespace core;
using namespace scene;

luaT_defMetaData(IGeometryCreator, false);

static int createCube(lua_State* L)
{
	IGeometryCreator* creator = checkarg<IGeometryCreator*>(L, 1);
	vector3df size;
	if (lua_gettop(L) == 4)
	{
		size.set(checkarg<float>(L, 2), 
			checkarg<float>(L, 3), checkarg<float>(L, 4));
	}
	else if (lua_gettop(L) == 2)
	{
		size = *checkarg<vector3df*>(L, 2);
	}
	else
	{
		luaL_error(L, "Args count don't match");
	}
	
	IMesh* mesh = creator->createCubeMesh(size);
	push<IMesh*>(L, mesh);

	return 1;
}

namespace xihad { namespace script
{
	int luaopen_Geometry(lua_State* L)
	{
		luaT_defRegsBgn(geomRegs)
			luaT_lnamedfunc(createCube),
		luaT_defRegsEnd
		MetatableFactory<IGeometryCreator>::create(L, geomRegs, 0);

		return 0;
	}
}}

