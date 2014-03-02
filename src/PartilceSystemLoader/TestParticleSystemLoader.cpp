#include <luaT/luaT.h>
#include <irrlicht/irrlicht.h>
#include <iostream>
#include "luaopen_ParticleSystem.h"
#include "CParticleSystemScriptFactory.h"

using namespace std;
using namespace irr;
using namespace xihad::particle;

namespace xihad { namespace particle
{
	struct MyReceiver : public IEventReceiver
	{
		MyReceiver() : pnode(0) {}

		~MyReceiver() 
		{
			removeNode();
			factory->drop();
		}

		void removeNode()
		{
			if (pnode)
			{
				pnode->clearParticles(); // optional
				pnode->remove();
			}

			pnode = 0;
		}

		bool OnEvent(const SEvent& event) 
		{
			if (event.KeyInput.PressedDown && event.KeyInput.Char == L' ')
			{
				removeNode();
				createNewParticleSystem();

				return true;
			}

			return false;
		}

		void createNewParticleSystem()
		{
			if (luaL_dofile(L, scriptPath) != 0)
			{
				cerr << lua_tostring(L, -1) << endl;
				return;
			}

			pnode = smgr->addParticleSystemSceneNode();
			luaT::push<decltype(pnode)>(L, pnode);
			luaT::push<decltype(factory)>(L, factory);

			if (lua_pcall(L, 2, 0, 0) != 0)
			{
				cout << lua_tostring(L, -1) << endl;
				pnode->remove();
				pnode = 0;
			}
			else
			{
				pnode->getMaterial(0).setTexture(0, 
					smgr->getVideoDriver()->getTexture("../Xihad/Assets/gfx/fire.bmp"));
				pnode->setMaterialFlag(video::EMF_LIGHTING, false);
				pnode->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
				pnode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
			}
		}

		scene::IParticleSystemSceneNode* pnode;
		const char* scriptPath;
		scene::ISceneManager* smgr;
		IParticleSystemScriptFactory* factory;
		lua_State* L;
	};
}}

int main(int argc, char** argv)
{
	IrrlichtDevice* device = createDevice(video::EDT_DIRECT3D9);

	MyReceiver* receiver = new MyReceiver;
	receiver->scriptPath = argc>1 ? argv[1] : "particle.lua";
	receiver->smgr = device->getSceneManager();
	receiver->factory = CParticleSystemScriptFactory::createDefault(receiver->smgr->getParticleSystemFactory());
	receiver->L = luaL_newstate();
	luaL_openlibs(receiver->L);
	luaopen_AllParticleSystem(receiver->L);

	receiver->createNewParticleSystem();
	receiver->smgr->addCameraSceneNode(0, core::vector3df(0,0,-20));
	device->setEventReceiver(receiver);
	while (device->run())
	{
		video::SColor bg = device->isWindowActive() ? video::SColor(0) : video::SColor(0xff101010);
		device->getVideoDriver()->beginScene(true, true, bg);
		device->getSceneManager()->drawAll();
		device->getVideoDriver()->endScene();
	}

	system("pause");
	return 0;
}
