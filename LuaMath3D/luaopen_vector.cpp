#include "stdafx.h"
#include <new>
#include <irrlicht/vector3d.h>
#include <irrlicht/quaternion.h>
#include "luaopen_util.h"

using namespace luaT;
using namespace irr::core;

luaT_defMetaData(vector3df, false);

namespace xihad { namespace script
{
	static int newVector3(lua_State* L)
	{
		int narg = lua_gettop(L);
		switch (narg)
		{
		case 0:
			return luaT_constructor(vector3df())(L);
		case 1:
			return luaT_constructor(vector3df(float))(L);
		case 3:
			return luaT_constructor(vector3df(float, float, float))(L);
		default:
			lua_error(L);
		}

		return 0;
	}

	static int vec3ToNumbers(lua_State* L)
	{
		vector3df* vec3 = checkarg<vector3df*>(L, 1);
		push<double>(L, vec3->X);
		push<double>(L, vec3->Y);
		push<double>(L, vec3->Z);
		return 3;
	}

	static int vec3Set(lua_State* L)
	{
		int narg = lua_gettop(L);
		vector3df* vec3 = checkarg<vector3df*>(L, 1);
		if (narg >= 2 && lua_isnumber(L, 2))
			vec3->X = (float) lua_tonumber(L, 2);

		if (narg >= 3 && lua_isnumber(L, 3))
			vec3->Y = (float) lua_tonumber(L, 3);

		if (narg >= 4 && lua_isnumber(L, 4))
			vec3->Z = (float) lua_tonumber(L, 4);

		return 0;
	}

	static int vec3Mul(lua_State* L)
	{
		luaL_checkany(L, 2);
		lua_CFunction func;
		if (lua_isnumber(L, 2)) // attempt to mul number
			func = luaT_mfunction_ovl(
			vector3df (vector3df::*)(float) const, vector3df::operator*);
		else // default as mul a vector
			func = BI_OPERATOR(vector3df, *);

		return func(L);
	}

	static int vec3Div(lua_State* L)
	{
		luaL_checkany(L, 2);
		lua_CFunction func;
		if (lua_isnumber(L, 2))
			func = luaT_mfunction_ovl(
			vector3df (vector3df::*)(float) const, vector3df::operator/);
		else
			func = BI_OPERATOR(vector3df, /);

		return func(L);
	}

	luaT_static void vec3Normal(vector3df* vec) 
	{
		vec->normalize();
	}}

	//! vector3d, axis, degree, (center)
	static int vec3Rotate(lua_State* L)
	{
		luaL_checkany(L, 3);
		vector3df* vec3 = checkarg<vector3df*>(L, 1);
		float degree = (float) luaL_checknumber(L, 3);

		vector3df center;
		if (lua_gettop(L) >= 4)
			center = *checkarg<vector3df*>(L, 4);

		if (lua_isstring(L, 2))
		{
			size_t sz;
			const char* arg = lua_tolstring(L, 2, &sz);
			char axis = sz == 1 ? arg[0] : '\0';
			switch (axis)
			{
			case 'x':
			case 'X':
				vec3->rotateYZBy(degree, center);
				break;
			case 'y':
			case 'Y':
				// rotateXZBy(-degree, center) 
				//			||
				// rotateZXBy(+degree, center)
				vec3->rotateXZBy(-degree, center);
				break;
			case 'z':
			case 'Z':
				vec3->rotateXYBy(degree, center);
				break;
			default:
				luaL_argerror(L, 2, "'X'/'x'/'Y'/'y'/'Z'/'z' only");
				break;
			}
		}
		else if (lua_isuserdata(L, 2))
		{
			vector3df* axis = checkarg<vector3df*>(L, 2);
			quaternion q;
			//matrix4 mat;
			q.fromAngleAxis(degree*DEGTORAD, *axis);
			q.normalize();
			(*vec3) -= center;
			(*vec3) = q * (*vec3);
			(*vec3) += center;
			//q.getMatrixCenter(mat, center, vector3df());
			//mat.transformVect(*vec3);
		}
		else
		{
			luaL_typerror(L, 2, "string/vector3d");
		}

		return 0;
	}

	int luaopen_vector(lua_State* L)
	{
		luaT_defRegsBgn(vctor)
			{ "vector", newVector3 },
		luaT_defRegsEnd
		luaL_register(L, "math3d", vctor);

		luaT_defRegsBgn(vec3Regs)
			{ "copy",	luaT_constructor(vector3df(const vector3df&)) },
			{ "xyz",	vec3ToNumbers },
			{ "set",	vec3Set },
			{ "length", luaT_mfunction(vector3df::getLength)	},	
			{ "length2",luaT_mfunction(vector3df::getLengthSQ)	},	
			{ "dot",	luaT_mfunction(vector3df::dotProduct)	},
			{ "cross",	luaT_mfunction(vector3df::crossProduct)	},
			{ "normalize",	luaT_cfunction(vec3Normal)					},
			{ "distance",	luaT_mfunction(vector3df::getDistanceFrom)},
			{ "distance2",	luaT_mfunction(vector3df::getDistanceFromSQ)},
			{ "rotate",		vec3Rotate },
			{ "horizontalAngle", luaT_mfunction(vector3df::getHorizontalAngle) },
			{ "__add", BI_OPERATOR(vector3df, +) },
			{ "__sub", BI_OPERATOR(vector3df, -) },
			{ "__mul", vec3Mul },
			{ "__div", vec3Div },
			{ "__unm", UN_OPERATOR(vector3df, -) },
			{ "__eq",  luaT_mfunction(vector3df::operator==) },
			{ "__le",  luaT_mfunction(vector3df::operator<=) },
			{ "__lt",  luaT_mfunction(vector3df::operator<) },
		luaT_defRegsEnd
		MetatableFactory<vector3df>::create(L, vec3Regs, 0);

		return 1;
	}
}}
