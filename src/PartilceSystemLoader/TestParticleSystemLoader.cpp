#include <luaT/luaT.h>
#include <irrlicht/irrlicht.h>
#include <iostream>

using namespace std;
using namespace irr;
int exportLibs(lua_State* L);
int main(int argc, char** argv)
{
	const char* scriptPath = "particle.lua";
	if (argc > 1)
		scriptPath = argv[1];

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	exportLibs(L);
	IrrlichtDevice* device = createDevice(video::EDT_DIRECT3D9);
	if (luaL_dofile(L, scriptPath) != 0)
	{
		cout << lua_tostring(L, -1) << endl;
	}
	else
	{
		auto pnode = device->getSceneManager()->addParticleSystemSceneNode();
		auto factory = device->getSceneManager()->getParticleSystemFactory();
		luaT::push<decltype(pnode)>(L, pnode);
		luaT::push<decltype(factory)>(L, factory);
		if (lua_pcall(L, 2, 0, 0) != 0)
		{
			cout << lua_tostring(L, -1) << endl;
		}
		else
		{
			pnode->getMaterial(0).Lighting = false;
			pnode->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
			pnode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
			device->getSceneManager()->addCameraSceneNode(0,core::vector3df(0,0,-20))->drop();
			while (device->run())
			{
				device->getVideoDriver()->beginScene(true, true, video::SColor(255,192,168,150));
				device->getSceneManager()->drawAll();
				device->getVideoDriver()->endScene();
			}
		}
	}

	system("pause");
	return 0;
}
