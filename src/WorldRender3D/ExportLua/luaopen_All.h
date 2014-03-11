#pragma once

struct lua_State;

namespace irr 
{
	class IrrlichtDevice;
}

namespace xihad { namespace render3d
{
	class IrrlichtComponentSystem;
	void luaopen_All(irr::IrrlichtDevice* dev, IrrlichtComponentSystem*, lua_State* L);
}}

