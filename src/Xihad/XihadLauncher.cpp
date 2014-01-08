#include <iostream>
#include <string>
#include "Engine\GameEngine.h"
#include "Engine\GameWorld.h"
#include "Engine\GameScene.h"
#include "Engine\GameObject.h"
#include "Engine\ComponentSystemRegistry.h"
#include "WorldRender3D\MeshComponent.h"
#include "WorldRender3D\IrrlichtComponentSystem.h"
#include "WorldRender3D\IrrlichtComponentSystemFactory.h"
#include "ScriptEngine\LuaComponentSystemFactory.h"
#include "WorldRender3D\RenderComponent.h"
#include "boost\variant\variant.hpp"
#include "Engine\Timeline.h"
#include "WorldRender3D\AnimatedMeshComponent.h"
#include "WorldRender3D\LightComponent.h"
#include "CppBase\XiAssert.h"
#include "WorldRender3D\Geometry.h"
#include "CreateDevice.h"
#include "irrlicht\IrrlichtDevice.h"
#include "irrlicht\ISceneManager.h"
#include "irrlicht\SLight.h"
#include <boost\property_tree\json_parser.hpp>
#include "Engine\MapProperties.h"
#include "XihadInitializer.h"
#include "irrlicht\irrlicht.h"

using namespace std;
using namespace irr;
using namespace scene;
using namespace core;
using namespace video;
using namespace xihad;
using namespace ngn;
using namespace script;
using namespace render3d;

using std::string;
GameEngine* engine = nullptr;

int main_xihad()
{
	using std::string;
	IrrlichtDevice* device = createDevice();
	initSystem(device);

	GameScene* scene = new GameScene;
	GameWorld* world = new GameWorld;
	world->setScene(scene);
	
	{
		MapProperties meshProps;
		meshProps["mesh"] = string("Assets/model/banana.b3d");

		GameObject* banana = scene->createObject("banana");
		RenderComponent* rc = dynamic_cast<RenderComponent*> (
					banana->appendComponent("Mesh", meshProps).first);
		banana->appendComponent("Luigi");
		{
			GameObject* banana2 = scene->createObject("banana2", banana);
			RenderComponent* rc2 = dynamic_cast<RenderComponent*> (
				banana2->appendComponent("Mesh", meshProps).first);
			banana2->setParent(banana);
			banana2->concatTranslate(2, 0, 0);
		}
	}
	
	{
		GameObject* camera = scene->createObject("camera");
		camera->appendComponent("Camera");
		camera->concatTranslate(0, 5, -20);
	}	

	{
		GameObject* sun = scene->createObject("sun");
		LightComponent* light = dynamic_cast<LightComponent*> (
			sun->appendComponent("Light").first);
		light->getLightData().AmbientColor = video::SColorf(.5f, .5f, .5f);
		sun->concatTranslate(0, 20, 10);
	}
	
	{
		GameObject* sky = scene->createObject("sky");
		MapProperties skydomeProp;
		skydomeProp["texture"] = string("Assets/gfx/skydome.jpg");
		skydomeProp["radius"] = 10.f;
		sky->appendComponent("SkyDome", skydomeProp);
	}

	{
		GameObject* ninja = scene->createObject("ninja");
		MapProperties ninjaProp;
		ninjaProp["mesh"] = string("Assets/model/ninja.b3d");
		ninjaProp["clips"] = string("Assets/model/ninja.clip");
		AnimatedMeshComponent* anim = dynamic_cast<AnimatedMeshComponent*> (
			ninja->appendComponent("AnimatedMesh", ninjaProp).first);
		anim->setDebugState(DebugState::EDS_SKELETON);
		anim->setFrameLoop(0, 300);
		{
			boost::property_tree::ptree tree;
	//////////////////////////////////////////////////////////////////////////
	//				animation framework
	//////////////////////////////////////////////////////////////////////////
			anim->playAnimation(tree.get<string>("walk"));
		}
		ninja->concatTranslate(5, 0, 0);
		ninja->concatUniScale(1.5f);
		ninja->concatRotate(0, 180, 0);
	}

	engine = new GameEngine(*world);
	XiAssert::isTrue(engine->initDevice(device));
	device->drop();

	engine->launch();
	delete engine;

	destroySystems();
	system("pause");
	return 0;
}