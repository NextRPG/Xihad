#include <luaT/luaT.h>
#include <irrlicht/irrlicht.h>
#include <iostream>
#include <WorldRender3D/ExportLua/luaopen_All.h>
#include <Particle/luaopen_ParticleSystem.h>
#include <Particle/CParticleSystemScriptFactory.h>
#include "ParticleEditorEnv.h"
#include <ctime>
#include <Particle/IStackPusherFactory.h>
#include "Particle/CTextureIniterStackPusherFactory.h"

using namespace std;
using namespace irr;
using namespace scene;
using namespace video;
using namespace xihad::particle;
using namespace editor;

namespace xihad { namespace particle { namespace editor
{
	struct ParticleEditorReceiver : public IEventReceiver
	{
		~ParticleEditorReceiver() 
		{
			removeNode();
			factory->drop();
		}

		void removeNode()
		{
			env->clearMessages();
			if (pnode)
			{
				pnode->remove();
				pnode = 0;
			}
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

			pnode = smgr->addParticleSystemSceneNode(65535);
			luaT::push<decltype(pnode.get())>(L, pnode.get());
			luaT::push<decltype(factory)>(L, factory);
			luaT::push<IParticleSystemLoaderEnv*>(L, env.get());

			if (lua_pcall(L, 3, 0, 0) != 0)
			{
				cerr << lua_tostring(L, -1) << endl;
				removeNode();
			}
		}

		irrptr<scene::IParticleSystemSceneNode> pnode;
		const char* scriptPath;
		scene::ISceneManager* smgr;
		irrptr<ParticleEditorEnv> env;
		CParticleSystemScriptFactory* factory;
		lua_State* L;
	};
}}}

static ISceneNode* addNinja(ISceneManager* smgr, core::vector3df pos)
{
	IAnimatedMeshSceneNode* ninja = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../Xihad/Assets/model/ninja.b3d"));
	ninja->setFrameLoop(182, 204);
	ninja->setMaterialFlag(video::EMF_LIGHTING, false);
	ninja->setAnimationSpeed(12);
	ninja->setPosition(pos);

	return ninja;
}

int main(int argc, char** argv)
{
	clock_t current = clock();
	IrrlichtDevice* device = createDevice(video::EDT_DIRECT3D9);

	ParticleEditorReceiver* receiver = new ParticleEditorReceiver;
	receiver->scriptPath = argc>1 ? argv[1] : "particle.lua";
	receiver->smgr = device->getSceneManager();
	receiver->factory = CParticleSystemScriptFactory::createDefault(receiver->smgr->getParticleSystemFactory());
// 	receiver->factory->Initers["Texture"] = irrptr<IStackPusherFactory>(
// 		new CTextureIniterStackPusherFactory(*device->getRandomizer(), *device->getVideoDriver()), false);
	receiver->L = luaL_newstate();

	{
		luaT::StackMemo m(receiver->L);
		luaL_openlibs(receiver->L);
		luaopen_AllParticleSystem(receiver->L);
		xihad::render3d::luaopen_Material(receiver->L);
		xihad::render3d::luaopen_SColor(receiver->L);
	}

	receiver->smgr->addSkyDomeSceneNode(device->getVideoDriver()->getTexture("../Xihad/Assets/gfx/skydome.jpg"));
	ISceneNode* source = addNinja(receiver->smgr, core::vector3df(-10, 0, 0));
	core::matrix4 rel = source->getRelativeTransformation();
	rel.setRotationDegrees(core::vector3df(0, 90, 0));
	source->setRelativeTransformation(rel);
	ISceneNode* target = addNinja(receiver->smgr, core::vector3df(10, 0, 0));
	rel = target->getRelativeTransformation();
	rel.setRotationDegrees(core::vector3df(0, -90, 0));
	target->setRelativeTransformation(rel);

	ParticleEditorEnv* env = new ParticleEditorEnv(source, target, device, current);
	receiver->env.reset(env);
	env->drop();

	receiver->createNewParticleSystem();
	receiver->smgr->addCameraSceneNode(0, core::vector3df(-12,12,-12), core::vector3df(5,0, 0));
	device->setEventReceiver(receiver);

	while (device->run())
	{
		current = clock();
		video::SColor bg = device->isWindowActive() ? video::SColor(0) : video::SColor(0xff101010);
		device->getVideoDriver()->beginScene(true, true, bg);
		device->getSceneManager()->onAnimate(current);
		receiver->env->update(current);
		device->getSceneManager()->drawAll();
		device->getVideoDriver()->endScene();
	}

	delete receiver;
	device->drop();
	system("pause");
	return 0;
}
