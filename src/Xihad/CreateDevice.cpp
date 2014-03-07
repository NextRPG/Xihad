#include "CreateDevice.h"
#include "irrlicht/SIrrCreationParameters.h"

namespace irr {
	extern "C" __declspec(dllimport) IrrlichtDevice* __cdecl createDeviceEx(
		const SIrrlichtCreationParameters& parameters);
}

irr::IrrlichtDevice* createDefaultDevice()
{
	irr::SIrrlichtCreationParameters param;
	param.WindowSize = irr::core::dimension2du(1024, 600);
	param.DriverType = irr::video::EDT_DIRECT3D9;

	param.Vsync = true;
	param.AntiAlias = 4;
	return irr::createDeviceEx(param);
}
