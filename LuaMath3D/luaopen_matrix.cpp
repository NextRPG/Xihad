#include "stdafx.h"
#include <irrlicht/matrix4.h>
#include "luaopen_util.h"

using namespace luaT;
using namespace irr::core;

namespace xihad { namespace script
{
	luaT_static int transform(lua_State* L)
	{
		luaT_variable(L, 1, matrix4*, _this);
		if (luaT_variable(L, 2, vector3df*, v2))
			_this->transformVect(*v2);
		else if (luaT_variable(L, 2, plane3df*, p2))
			_this->transformPlane(*p2);
		else if (luaT_variable(L, 2, aabbox3df*, b2))
			_this->transformBoxEx(*b2);
		else 
			luaL_typerror(L, 2, "vector/plane/aabb");

		return 0;
	}}

	int luaopen_matrix(lua_State* L)
	{
		luaT_defRegsBgn(mregs)
			{ "copy", luaT_constructor(matrix4(const matrix4&)) },
			luaT_mnamedfunc(matrix4, getTranslation),
			luaT_mnamedfunc(matrix4, getScale),
			luaT_mnnamefunc(matrix4, getRotationDegrees, getRotation),
			luaT_mnamedfunc(matrix4, isOrthogonal),
			luaT_mnamedfunc(matrix4, isIdentity),
			luaT_mnamedfunc_nret(matrix4, setTranslation),
			luaT_mnnamefunc_nret(matrix4, setRotationDegrees, setRotation),
			// setScale()
			luaT_mnamedfunc_nret(matrix4, makeIdentity),
			luaT_mnamedfunc_nret(matrix4, makeInverse),
			luaT_cnamedfunc(transform),
		luaT_defRegsEnd

		// TODO math3d.matrix
		return 1;
	}
}}