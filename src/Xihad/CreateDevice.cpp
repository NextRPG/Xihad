#include "CreateDevice.h"
#include "irrlicht\irrlicht.h"

irr::IrrlichtDevice* createDefaultDevice()
{
	irr::SIrrlichtCreationParameters param;
	param.WindowSize = irr::core::dimension2du(1024, 600);
	param.DriverType = irr::video::EDT_DIRECT3D9;

	param.Vsync = true;
	param.AntiAlias = 4;
	return irr::createDeviceEx(param);
}
