#include "CreateDevice.h"
#include "irrlicht\irrlicht.h"

irr::IrrlichtDevice* createDefaultDevice()
{
	return irr::createDevice(irr::video::EDT_OPENGL, 
		irr::core::dimension2d<size_t>(1024, 600), 16,
		false, true, false, 0);
}
