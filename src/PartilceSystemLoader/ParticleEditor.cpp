/**
 * 自定义 Affector 的步骤（假设为 PauseAffector）：
 * 1. 实现 IParticleAffector 子类 ParticlePauseAffector，定义默认构造函数
 * 
 * 2. 向 Lua 中注册该类。需要在合适的 cpp 文件中添加如下代码（可以是 ParticlePauseAffector.cpp / xxx_metadata.cpp）
 *	<p>
 *		#include <luaT/luaT.h>
 *		#include "ParticlePauseAffector.h"
 *		luaT_defMetaData(ParticlePauseAffector, false);
 *	
 * 3. 导出新类的成员函数供 Lua 在脚本中调用。假设类成员函数的参数、返回值都是原始类型或vector3df等，代码示例如下：
 *	<p>
 *		luaT_defRegsBgn(arbitrary_name)
 *			luaT_mnamedfunc(ParticlePauseAffector, member_function_name),
 *			// ....,
 *			// ....,
 *		luaT_defRegsEnd
 *		luaT::MetatableFactory<ParticlePauseAffector, IPartilceAffector>::create(L, arbitrary_name);
 *	<p> 
 *		可以参考 luaopen_ParticleSystem.cpp 中的格式
 *		
 *		
 * 4. 通过如下代码注册新的 Affector
 *	<p>
 *		IStackPusherFactory* f = new DefaultStackPusherFactory<ParticlePauseAffector>;
 *		receiver->factory->Affectors["Pause"] = f;
 *		f->drop();
 */

#include <luaT/luaT.h>
#include <irrlicht/irrlicht.h>
#include <iostream>
#include "luaopen_ParticleSystem.h"
#include "CParticleSystemScriptFactory.h"
#include "CParticleEditorEnv.h"

using namespace std;
using namespace irr;
using namespace scene;
using namespace video;
using namespace xihad::particle;

namespace xihad { namespace particle
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
			luaT::push<decltype(env.get())>(L, env.get());

			if (lua_pcall(L, 3, 0, 0) != 0)
			{
				cerr << lua_tostring(L, -1) << endl;
				removeNode();
			}
		}

		irrptr<scene::IParticleSystemSceneNode> pnode;
		const char* scriptPath;
		scene::ISceneManager* smgr;
		irrptr<IParticleSystemLoaderEnv> env;
		IParticleSystemScriptFactory* factory;
		lua_State* L;
	};
}}

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
	IrrlichtDevice* device = createDevice(video::EDT_DIRECT3D9);

	ParticleEditorReceiver* receiver = new ParticleEditorReceiver;
	receiver->scriptPath = argc>1 ? argv[1] : "particle.lua";
	receiver->smgr = device->getSceneManager();
	receiver->factory = CParticleSystemScriptFactory::createDefault(receiver->smgr->getParticleSystemFactory());
	receiver->L = luaL_newstate();
	luaL_openlibs(receiver->L);
	luaopen_AllParticleSystem(receiver->L);

	receiver->smgr->addSkyDomeSceneNode(device->getVideoDriver()->getTexture("../Xihad/Assets/gfx/skydome.jpg"));
	ISceneNode* source = addNinja(receiver->smgr, core::vector3df(-10, 0, 0));
	source->setRotation(core::vector3df(0, 90, 0));
	ISceneNode* target = addNinja(receiver->smgr, core::vector3df(10, 0, 0));
	target->setRotation(core::vector3df(0, -90, 0));

	receiver->env = irrptr<IParticleSystemLoaderEnv>(new CParticleEditorEnv(source, target), false);

	receiver->createNewParticleSystem();
	receiver->smgr->addCameraSceneNode(0, core::vector3df(-12,12,-12), core::vector3df(5,0, 0));
	device->setEventReceiver(receiver);
	while (device->run())
	{
		video::SColor bg = device->isWindowActive() ? video::SColor(0) : video::SColor(0xff101010);
		device->getVideoDriver()->beginScene(true, true, bg);
		device->getSceneManager()->drawAll();
		device->getVideoDriver()->endScene();
	}

	delete receiver;
	device->drop();
	system("pause");
	return 0;
}
