#include "CreateDevice.h"
#include "irrlicht/SIrrCreationParameters.h"
#include <irrlicht/SMaterial.h>

namespace irr {
	extern "C" __declspec(dllimport) IrrlichtDevice* __cdecl createDeviceEx(
		const SIrrlichtCreationParameters& parameters);
}

using namespace irr;
IrrlichtDevice* createDefaultDevice()
{
// 	SIrrlichtCreationParameters p;
// 	p.DriverType = video::EDT_DIRECT3D9;
// 	p.WindowSize = core::dimension2du(1024, 600);
// 	p.Stencilbuffer = false;
// 	p.Vsync = false;

	irr::SIrrlichtCreationParameters param;


	// D3D9 + AntiAlias + RenderToTexture = No Z buffer
	param.DriverType = video::EDT_OPENGL;
	param.WindowSize = core::dimension2du(1024, 600);

	param.Stencilbuffer = true;
	param.Vsync = false;
	param.AntiAlias = video::EAAM_QUALITY;
	return irr::createDeviceEx(param);
}
