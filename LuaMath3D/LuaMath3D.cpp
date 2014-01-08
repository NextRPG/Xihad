// LuaMath3D.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "luaopen_util.h"
#include "irrlicht/vector3d.h"
#include "irrlicht/quaternion.h"
#include <iostream>

using namespace irr::core;
using namespace luaT;

namespace xihad { namespace script
{
	static int mathLerp(lua_State* L)
	{
		// TODO is<vector3df*> 不可以
		if (checkarg<vector3df*>(L, 1))
		{
			return luaT_mfunction(vector3df::getInterpolated)(L);
		}
		else if (quaternion* q1 = checkarg<quaternion*>(L, 1))
		{
			quaternion* q2 = checkarg<quaternion*>(L, 2);
			float ratio = checkarg<float>(L, 3);
			quaternion result;
			result.lerp(*q1, *q2, ratio);
			push<quaternion>(L, result);
		}
		else
		{
			luaL_typerror(L, 1, "Not lerpable");
		}

		return 1;
	}

	static int mathIntersects(lua_State* L)
	{
		if (luaT_variable(L, 1, plane3df*, p1))
		{
			if (luaT_variable(L, 2, plane3df*, p2))
			{
				push<bool>(L, p1->existsIntersection(*p2));
				return 1;
			}
			else if (luaT_variable(L, 2, line3df*, l2))
			{
				float ratio = p1->getKnownIntersectionWithLine(l2->start, l2->end);
				push<bool>(L, ratio>=0 && ratio < 1);
				return 1;
			}
		}
		else if (luaT_variable(L, 1, aabbox3df*, b1))
		{
			if (luaT_variable(L, 2, aabbox3df*, b2))
			{
				push<bool>(L, b1->intersectsWithBox(*b2));
				return 1;
			}
			else if (luaT_variable(L, 2, plane3df*, p2))
			{
				EIntersectionRelation3D e = b1->classifyPlaneRelation(*p2);
				push<bool>(L, e==ISREL3D_CLIPPED);
				return 1;
			}
			else if (luaT_variable(L, 2, line3df*, l2))
			{
				push<bool>(L, b1->intersectsWithLine(*l2));
				return 1;
			}
		}

		luaL_error(L, "Arguments type dismatch for math3d.intersects, "
			"only supports plane2[plane/line], aabb2[aabb, plane, line] and order should match");

		return 0;
	}

	static int mathIntersection(lua_State* L)
	{
		if (luaT_variable(L, 1, plane3df*, p1))
		{
			if (luaT_variable(L, 2, plane3df*, p2))
			{
				vector3df p, v;
				if (p1->getIntersectionWithPlane(*p2, p, v))
				{
					push<vector3df>(L, p);
					push<vector3df>(L, v);
					return 2;
				}
				else
				{
					return 0;
				}
			}
			else if (luaT_variable(L, 2, line3df*, l2))
			{
				vector3df out;
				if (p1->getIntersectionWithLimitedLine(l2->start, l2->end, out))
				{
					push<vector3df>(L, out);
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else if (luaT_variable(L, 2, vector3df*, p2))
			{
				luaT_variable(L, 3, vector3df*, v3);

				vector3df out;
				if (p1->getIntersectionWithLine(*p2, *v3, out))
				{
					push<vector3df>(L, out);
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}

		luaL_error(L, "Arguments type dismatch for math3d.intersection, "
			"only supports plane2[plane/line/point-vector] and order should match");
		return 0;
	}

	static luaL_Reg m3d_libs[] = {
		{ "lerp",		mathLerp },			
		{ "intersects", mathIntersects },	// -> bool
		{ "intersects", mathIntersection },	// -> vector[2] or vector
	};

	extern int luaopen_vector(lua_State* L);
	extern int luaopen_quaternion(lua_State* L);
	extern int luaopen_aabb(lua_State* L);
	extern int luaopen_line(lua_State* L);
	extern int luaopen_plane(lua_State* L);
	extern int luaopen_matrix(lua_State* L);
	static int luaopen_math3d_impl(lua_State* L)
	{
		// stack recover
		{
			StackMemo memo(L);
			luaopen_vector(L);
			luaopen_quaternion(L);
			luaopen_aabb(L);
			luaopen_line(L);
			luaopen_plane(L);
			//luaopen_matrix(L);
		}

		luaL_register(L, "math3d", m3d_libs);

		return 1;
	}

	extern "C" __declspec(dllexport) int luaopen_math3d(lua_State* L)
	{
		return luaopen_math3d_impl(L);
	}
}}
