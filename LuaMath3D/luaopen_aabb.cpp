#include "stdafx.h"
#include <new>
#include <irrlicht/aabbox3d.h>
#include "luaopen_util.h"

using namespace luaT;
using namespace irr::core;
namespace xihad { namespace script 
{
	luaT_static int boxAddPoint(lua_State* L)
	{
		if (lua_gettop(L) == 2)
		{
			return luaT_mfunction_ovl(
				void (aabbox3df::*)(const vector3df&), 
				aabbox3df::addInternalPoint)(L);
		}
		else
		{
			luaL_checkany(L, 4);
			return luaT_mfunction_ovl(
				void (aabbox3df::*)(float, float, float), 
				aabbox3df::addInternalPoint)(L);
		}
	}}

	// point, line, box
	luaT_static int boxContains(lua_State* L)
	{
		auto box = checkarg<aabbox3df*>(L, 1);
		bool contains;

		if (lua_gettop(L) == 4)
		{
			vector3df point(checkarg<float>(L, 2), 
				checkarg<float>(L, 3), checkarg<float>(L, 4));
			contains = box->isPointInside(point);
		}
		else if (vector3df* point = checkarg<vector3df*>(L, 2))
		{
			contains = box->isPointInside(*point);
		}
		else if (aabbox3df* other = checkarg<aabbox3df*>(L, 2))
		{
			contains = other->isFullInside(*box);
		}
		else if (line3df* line = checkarg<line3df*>(L, 2))
		{
			aabbox3df buf(line->start, line->end);
			buf.repair();
			contains = buf.isFullInside(*box);
		}
		else
		{
			luaL_typerror(L, 2, "point/box/line");
		}

		push<bool>(L, contains);
		return 1;
	}}

	luaT_static inline bool boxFullContainsBox(aabbox3df* a, aabbox3df* b)
	{
		return 
			a->MinEdge.X < b->MinEdge.X && 
			a->MinEdge.Y < b->MinEdge.Y && 
			a->MinEdge.Z < b->MinEdge.Z &&
			a->MaxEdge.X > b->MaxEdge.X && 
			a->MaxEdge.Y > b->MaxEdge.Y && 
			a->MaxEdge.Z > b->MaxEdge.Z;
	}}

	// point, line, box
	luaT_static int boxFullContains(lua_State* L)
	{
		auto box = checkarg<aabbox3df*>(L, 1);
		bool contains;

		if (lua_gettop(L) == 4)
		{
			vector3df point(checkarg<float>(L, 2), 
				checkarg<float>(L, 3), checkarg<float>(L, 4));
			contains = box->isPointTotalInside(point);
		}
		else if (vector3df* point = checkarg<vector3df*>(L, 2))
		{
			contains = box->isPointTotalInside(*point);
		}
		else if (aabbox3df* other = checkarg<aabbox3df*>(L, 2))
		{
			contains = boxFullContainsBox(box, other);
		}
		else if (line3df* line = checkarg<line3df*>(L, 2))
		{
			aabbox3df buf(line->start, line->end);
			buf.repair();
			contains = boxFullContainsBox(box, &buf);
		}
		else
		{
			luaL_typerror(L, 2, "point/box/line");
		}

		push<bool>(L, contains);
		return 1;
	}}

	// line, box, plane
	luaT_static int boxIntersects(lua_State* L)
	{
		luaL_checkany(L, 2);
		aabbox3df* box = checkarg<aabbox3df*>(L, 1);

		bool intersects;
		if (aabbox3df* other = checkarg<aabbox3df*>(L, 2))
			intersects = box->intersectsWithBox(*other);
		else if (line3df* line = checkarg<line3df*>(L, 2))
			intersects = box->intersectsWithLine(*line);
		else if (plane3df* plane = checkarg<plane3df*>(L, 2))
			intersects = (box->classifyPlaneRelation(*plane) == ISREL3D_CLIPPED);
		else
			luaL_typerror(L, 2, "box/line");

		push<bool>(L, intersects);
		return 1;
	}}

	int luaopen_aabb(lua_State* L)
	{
		luaT_defRegsBgn(actor)
			{ "aabb", luaT_constructor(aabbox3df(const vector3df&)) },
		luaT_defRegsEnd
		luaL_register(L, "math3d", actor);

		luaT_defRegsBgn(qregs)
			{ "copy", luaT_constructor(aabbox3df(const aabbox3df&)) },
			luaT_mnnamefunc(aabbox3df, getCenter, center),
			luaT_mnnamefunc(aabbox3df, getExtent, extent),
			luaT_mnnamefunc(aabbox3df, getVolume, volume),
			luaT_mnnamefunc(aabbox3df, isEmpty,	empty),
			luaT_mnnamefunc(aabbox3df, getArea, area),
			luaT_mnnamefunc(aabbox3df, addInternalBox, addBox),
			luaT_mnnamefunc(aabbox3df, operator==, __eq),
			luaT_lnnamefunc(boxAddPoint,	addPoint),
			luaT_lnnamefunc(boxContains,	contains),
			luaT_lnnamefunc(boxFullContains,fullContains),
			luaT_lnnamefunc(boxIntersects,	intersects),
			luaT_mnname_readattr(aabbox3df, MinEdge, minEdge),
			luaT_mnname_readattr(aabbox3df, MaxEdge, maxEdge),
		luaT_defRegsEnd
		MetatableFactory<aabbox3df>::create(L, qregs, 0);

		return 1;
	}
}}
