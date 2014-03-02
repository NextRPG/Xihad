#include "stdafx.h"
#include <luaT/luaT.h>
#include <irrlicht/matrix4.h>
#include <irrlicht/quaternion.h>

using namespace irr::core;
luaT_defMetaData(line3df,	false);
luaT_defMetaData(aabbox3df, false);
luaT_defMetaData(matrix4,	false);
luaT_defMetaData(plane3df,	false);
luaT_defMetaData(quaternion,false);
luaT_defMetaData(vector3df, false);
