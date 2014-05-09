#include <luaT/luaT.h>
#include <irrlicht/SColor.h>

using namespace irr;
using namespace video;
using namespace luaT;
namespace xihad { namespace render3d
{
	static void setColor(SColor* color, lua_State* L, int _1)
	{
		luaL_checkany(L, _1 + 1);
		if (lua_gettop(L) == _1 + 1)
		{
			if (lua_isnumber(L, _1+1))
			{
				luaT_variable(L, _1 + 1, u32, c);
				color->set(c);
			}
			else
			{
				luaT_variable(L, _1+1, const SColor&, cpy);
				*color = cpy;
			}
		}
		else 
		{
			luaL_checkany(L, _1 + 4);
			luaT_variable(L, _1 + 1, u32, a);
			luaT_variable(L, _1 + 2, u32, r);
			luaT_variable(L, _1 + 3, u32, g);
			luaT_variable(L, _1 + 4, u32, b);
			color->set(a, r, g, b);
		}
	}

	static int set(lua_State* L)
	{
		luaT_variable(L, 1, SColor*, color);
		setColor(color, L, 1);
		return 0;
	}

	static int newColor(lua_State* L)
	{
		SColor data;
		if (lua_gettop(L) > 0)
			setColor(&data, L, 0);

		luaT::newUserdata<SColor>(L, data);
		return 1;
	}

	int luaopen_SColor(lua_State* L)
	{
		luaT_defRegsBgn(scolor)
			luaT_mnamedfunc(SColor, getAlpha),
			luaT_mnamedfunc(SColor, getRed),
			luaT_mnamedfunc(SColor, getGreen),
			luaT_mnamedfunc(SColor, getBlue),
			luaT_mnamedfunc(SColor, getLightness),
			luaT_mnamedfunc(SColor, getLuminance),
			luaT_mnamedfunc(SColor, setAlpha),
			luaT_mnamedfunc(SColor, setRed),
			luaT_mnamedfunc(SColor, setGreen),
			luaT_mnamedfunc(SColor, setBlue),
			luaT_lnamedfunc(set),
		luaT_defRegsEnd
		MetatableFactory<SColor>::create(L, scolor);

		luaT_defRegsBgn(colorTable)
			luaT_lnnamefunc(newColor, new),
		luaT_defRegsEnd
		luaL_register(L, "Color", colorTable);

		setField(L, -1, "white", 	0xffffffff);
		setField(L, -1, "black", 	0xff000000);
		setField(L, -1, "red",	 	0xffff0000);
		setField(L, -1, "green", 	0xff00ff00);
		setField(L, -1, "blue",	 	0xff0000ff);
		setField(L, -1, "yellow",	0xffffff00);
		setField(L, -1, "cyan",  	0xff00ffff);
		setField(L, -1, "magenta",	0xffff00ff);
		setField(L, -1, "gray",		0xff808080);
		setField(L, -1, "darkGray",	0xffa9a9a9);
		setField(L, -1, "lightGray",0xffd3d3d3);
		setField(L, -1, "brown",	0xffa52a2a);
		setField(L, -1, "snow",		0xfffffafa);
		setField(L, -1, "orange",	0xffffa500);
		setField(L, -1, "dimGray",	0xff696969);
		setField(L, -1, "gold",		0xffffd700);
		setField(L, -1, "silver",	0xffc0c0c0);
		
		setField(L, -1, "transparent", 0x00000000);

		return 1;
	}
}}
