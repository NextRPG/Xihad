#pragma once

namespace irr
{
	class IrrlichtDevice;
}

namespace xihad
{
	void initSystem(irr::IrrlichtDevice* device);
	void destroySystems();
}

