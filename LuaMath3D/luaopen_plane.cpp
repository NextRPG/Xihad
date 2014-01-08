#include "stdafx.h"
#include <irrlicht/plane3d.h>
#include "luaopen_util.h"

using namespace luaT;
using namespace irr::core;

luaT_defMetaData(plane3df,	false);

namespace xihad { namespace script
{
	static int setPlane(lua_State* L)
	{
		int narg = lua_gettop(L);
		switch (narg)
		{
		case 3:
			if (lua_isnumber(L, 3))
				return luaT_mfunction_ovl(
					void (plane3df::*)(const vector3df&, float),
					plane3df::setPlane)(L);
			else
				return luaT_mfunction_ovl(
					void (plane3df::*)(const vector3df&, const vector3df&),
					plane3df::setPlane)(L);
		case 4:
			return luaT_mfunction_ovl(
				void (plane3df::*)(const vector3df&, const vector3df&, const vector3df&), 
				plane3df::setPlane)(L);
		default:
			luaL_error(L, "arguments count don't match");
			break;
		}

		return 0;
	}

	static int newPlane(lua_State* L)
	{
		int narg = lua_gettop(L);
		switch (narg)
		{
		case 2:
			if (lua_isnumber(L, 2))
				return luaT_constructor(
					plane3df(const vector3df&, const float))(L);
			else
				return luaT_constructor(
					plane3df(const vector3df&, const vector3df&))(L);
		case 3:
			return luaT_constructor(
				plane3df(const vector3df&, const vector3df&, const vector3df&))(L);
		default:
			luaL_error(L, "arguments count don't match");
			break;
		}

		return 0;
	}
	const char* relationToPoint(plane3df* plane, const vector3df& point)
	{
		EIntersectionRelation3D relation = plane->classifyPointRelation(point);
		switch (relation)
		{
		case irr::core::ISREL3D_FRONT:
			return "front";
		case irr::core::ISREL3D_BACK:
			return "back";
		case irr::core::ISREL3D_PLANAR:
			return "planar";
		default:
			return NULL;	// impossible
		}
	}

	int luaopen_plane(lua_State* L)
	{
		luaT_defRegsBgn(pctor)
			{ "plane", newPlane },
		luaT_defRegsEnd
		luaL_register(L, "math3d", pctor);

		luaT_defRegsBgn(planeRegs)
			{ "copy", luaT_constructor(plane3df(const plane3df&)) },
			luaT_mnnamefunc(plane3df, operator==, __eq),
			luaT_mnamedfunc(plane3df, isFrontFacing),
			luaT_mnnamefunc(plane3df, getDistanceTo, distanceToPoint),
			luaT_mnnamefunc(plane3df, getMemberPoint, memberPoint),
			luaT_lnnamefunc(setPlane, set),
			luaT_cnamedfunc(relationToPoint),
		luaT_defRegsEnd
		MetatableFactory<plane3df>::create(L, planeRegs, 0);

		return 1;
	}
}}


