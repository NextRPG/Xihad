#include "stdafx.h"
#include <irrlicht/line3d.h>
#include "luaopen_util.h"

using namespace luaT;
using namespace irr::core;

namespace xihad { namespace script 
{
#ifdef LUAOPEN_LINE_INTERSECTS_SPHERE
	luaT_static int lineInterWithSphere(lua_State* L)
	{
		line3df* line = checkarg<line3df*>(L, 1);
		vector3df* origin = checkarg<vector3df*>(L, 2);
		float radius = checkarg<float>(L, 2);

		irr::f64 dst;
		bool intersect = line->getIntersectionWithSphere(*origin, radius, dst);

		push<bool>(L, intersect);
		if (intersect)
			push<double>(L, dst);

		return intersect ? 2 : 1;
	}}
#endif // LUAOPEN_LINE_INTERSECTS_SPHERE

	int luaopen_line(lua_State* L)
	{
		luaT_defRegsBgn(lctor)
			{ "line", luaT_constructor(line3df(const vector3df& begin, const vector3df& finish)) },
		luaT_defRegsEnd
		luaL_register(L, "math3d", lctor);

		luaT_defRegsBgn(lineRegs)
			{ "copy", luaT_constructor(line3df(const line3df&)) },
			luaT_mnnamefunc(line3df, getVector, vector),
			luaT_mnnamefunc(line3df, getMiddle, middle),
			luaT_mnnamefunc(line3df, getLength, length),
			luaT_mnnamefunc(line3df, getLengthSQ, length2),
			luaT_mnnamefunc(line3df, getClosestPoint, closestP2P),
			luaT_mnnamefunc(line3df, operator==, __eq),
			luaT_mnamed_writeattr(line3df, start),
			luaT_mnname_writeattr(line3df, end, finish),
	#ifdef LUAOPEN_LINE_INTERSECTS_SPHERE
			luaT_cnnamefunc(lineInterWithSphere, intersectsSphere),
	#endif
		luaT_defRegsEnd
		MetatableFactory<line3df>::create(L, lineRegs, 0);

		return 1;
	}
}}
