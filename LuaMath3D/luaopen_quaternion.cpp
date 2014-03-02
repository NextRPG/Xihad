#include "stdafx.h"
#include <new>
#include <irrlicht/quaternion.h>
#include "luaopen_util.h"

using namespace luaT;
using namespace irr::core;

namespace xihad { namespace script
{
	static void setQuaternion(lua_State* L, int _1, quaternion* q)
	{
		luaL_checkany(L, _1);
		int n = lua_gettop(L);

		if (n == _1 + 2) // 3 个参数
		{
			// from euler
			float rx = checkarg<float>(L, _1 + 0);
			float ry = checkarg<float>(L, _1 + 1);
			float rz = checkarg<float>(L, _1 + 2);
			q->set(rx*DEGTORAD, ry*DEGTORAD, rz*DEGTORAD); // already normalized
		}
		else if (n == _1 + 1) // 2 个参数
		{
			if (lua_isnumber(L, _1))
			{
				// from axis
				float angle = checkarg<float>(L, _1);
				vector3df* axis = checkarg<vector3df*>(L, _1 + 1);
				q->fromAngleAxis(angle*DEGTORAD, *axis);
				q->normalize();
			}
			else
			{
				// rotation from to
				vector3df* from = checkarg<vector3df*>(L, _1);
				vector3df* to = checkarg<vector3df*>(L, _1 + 1);
				q->rotationFromTo(*from, *to);	// normalized
			}
		}
		else if (n == _1 + 3) // 4 个参数
		{
			float x = checkarg<float>(L, _1 + 0);
			float y = checkarg<float>(L, _1 + 1);
			float z = checkarg<float>(L, _1 + 2);
			float w = checkarg<float>(L, _1 + 3);
			q->set(x, y, z, w);
		}
	}

	static int newQuatertion(lua_State* L)
	{
		quaternion qbuf;
		if (lua_gettop(L) > 0)
			setQuaternion(L, 1, &qbuf);

		void* mem = lua_newuserdata(L, sizeof(quaternion));
		quaternion* q = new (mem) quaternion(qbuf);
		Metatable::bind<quaternion>(L, -1);

		return 1;
	}

	luaT_static quaternion lerpTo(quaternion* from, quaternion* to, double time)
	{
		quaternion ret;
		ret.lerp(*from, *to, (float) time);
		ret.normalize();
		return ret;
	}}

	luaT_static quaternion slerpTo(quaternion* from, quaternion* to, double time)
	{
		quaternion ret;
		ret.slerp(*from, *to, (float) time);
		ret.normalize();
		return ret;
	}}

	static int toEuler(lua_State* L)
	{
		auto q = checkarg<quaternion*>(L, 1);
		vector3df v;
		q->toEuler(v);
		v *= RADTODEG;
		push<float>(L, v.X);
		push<float>(L, v.Y);
		push<float>(L, v.Z);
		return 3;
	}

	static int toAngleAxis(lua_State* L)
	{
		auto q = checkarg<quaternion*>(L, 1);

		float angle; 
		vector3df axis;
		q->toAngleAxis(angle, axis);
		push<float>(L, angle*RADTODEG);
		push<vector3df>(L, axis);
		return 2;
	}

	static int set(lua_State* L)
	{
		auto q = checkarg<quaternion*>(L, 1);
		setQuaternion(L, 2, q);
		return 0;
	}

	static int xyzw(lua_State* L)
	{
		luaT_variable(L, 1, quaternion*, q);
		push<float>(L, q->X);
		push<float>(L, q->Y);
		push<float>(L, q->Z);
		push<float>(L, q->W);
		return 4;
	}

	int luaopen_quaternion(lua_State* L)
	{
		luaT_defRegsBgn(qctor)
			{ "quaternion", newQuatertion },
		luaT_defRegsEnd
		luaL_register(L, "math3d", qctor);

		// TODO: getMatrix, getMatrixCenter, getMatrix_transposed
		luaT_defRegsBgn(qregs)
			{ "copy",		luaT_constructor(quaternion(const quaternion&)) },
			luaT_cnamedfunc(lerpTo),
			luaT_cnamedfunc(slerpTo),
			luaT_lnamedfunc(toEuler),
			luaT_lnamedfunc(toAngleAxis),
			luaT_lnamedfunc(set),
			luaT_mnamedfunc_nret(quaternion, makeIdentity),
			luaT_mnamedfunc_nret(quaternion, makeInverse),
			luaT_mnamedfunc_nret(quaternion, normalize),
			luaT_mnnamefunc(quaternion, operator==, __eq),
			luaT_lnamedfunc(xyzw),
			{ "__mul",	luaT_mfunction_ovl(
							vector3df (quaternion::*)(const vector3df&) const, 
							quaternion::operator *) },
		luaT_defRegsEnd
		MetatableFactory<quaternion>::create(L, qregs, 0);

		return 1;
	}
}}
