#include "IrrlichtComponentSystemFactory.h"
#include <irrlicht/IrrlichtDevice.h>
#include <irrlicht/ISceneManager.h>
#include <ScriptEngine/LuaComponentSystem.h>

#include "IrrlichtComponentSystem.h"
#include "Engine/GameScene.h"
#include "ExportLua/luaopen_All.h"
#include "AnimationClipsJsonLoader.h"

using namespace xihad::ngn;
using namespace irr;
using namespace std;
namespace xihad { namespace render3d
{
	IrrlichtComponentSystemFactory::IrrlichtComponentSystemFactory(IrrlichtDevice* device) :
		MultiComponentSystemFactory("Render"), mDevice(device)
	{
		xassert(mDevice);
		AnimationClipsLoader* loader = new AnimationClipsJsonLoader;
		mCachedClips.setAnimationClipsLoader(loader);
		loader->drop();
		init();
	}

	void IrrlichtComponentSystemFactory::init()
	{
		registerDerivedType("Mesh"			);
		registerDerivedType("SkyDome"		);
		registerDerivedType("Billboard"		);
		registerDerivedType("Camera"		);
		registerDerivedType("Light"			);
		registerDerivedType("Terrian"		);
		registerDerivedType("Text"			);
		registerDerivedType("AnimatedMesh"	);
		registerDerivedType("ParticleSystem");
	}

	irr::IrrlichtDevice* IrrlichtComponentSystemFactory::getDevice()
	{
		return mDevice.get();
	}

	ngn::ComponentSystem* IrrlichtComponentSystemFactory::createMainSystem(ngn::GameScene* scene)
	{
		scene::ISceneManager* newScene = mDevice->getSceneManager()->createNewSceneManager(false);

		// TODO remove the initialize process
		float ambient = .3f;
		newScene->setAmbientLight(video::SColorf(ambient, ambient, ambient));
		newScene->setShadowColor(video::SColor(60,0,0,0));
		
		auto sys = new IrrlichtComponentSystem(mDevice.get(), newScene, *this, &mCachedClips);
		if (sys && scene->hasSystem("Lua"))
		{
			auto lcs = static_cast<script::LuaComponentSystem*>(scene->requireSystem("Lua"));
			luaopen_All(mDevice.get(), sys, lcs->getLuaState());
		}

		newScene->drop();
		return sys;
	}

}}