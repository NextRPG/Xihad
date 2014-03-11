#include "IrrlichtComponentSystem.h"
#include "CppBase\xassert.h"
#include <string>
#include <irrlicht\ISceneManager.h>
#include <irrlicht\IrrlichtDevice.h>
#include <irrlicht\ICameraSceneNode.h>
#include "Engine\Timeline.h"
#include "MeshComponent.h"
#include "LightComponent.h"
#include "CameraComponent.h"
#include "Engine\Properties.h"
#include "boost\variant\get.hpp"
#include "CppBase\StdMap.h"
#include "Engine\irr_ptr.h"
#include "AnimatedMeshComponent.h"
#include "Engine\dimension2d.h"
#include "BillboardComponent.h"
#include <boost\property_tree\json_parser.hpp>
#include "ParticleSystemComponent.h"
#include "TextureManager.h"
#include "MeshManager.h"

using namespace xihad::ngn;
using namespace irr;
using namespace scene;
using namespace video;
using namespace std;
using namespace boost;
namespace xihad { namespace render3d
{
	struct IrrlichtComponentSystem::impl
	{
		irr_ptr<IrrlichtDevice> device;
		irr_ptr<IVideoDriver> driver;
		irr_ptr<ISceneManager> smgr;
		AnimationClipsCache* clipCaches;
		boost::scoped_ptr<TextureManager> texManager;
		boost::scoped_ptr<MeshManager> meshManager;

		list<RenderComponent*> renderComponents;
	};

	IrrlichtComponentSystem::IrrlichtComponentSystem( 
		IrrlichtDevice* device, ISceneManager* scene, 
		const InheritanceTree& tree, AnimationClipsCache& gCache) :
	BaseComponentSystem(tree), mImpl(new impl)
	{
		xassert(device);
		xassert(scene);

		mImpl->device = device;
		mImpl->driver = device->getVideoDriver();
		mImpl->smgr = scene;
		mImpl->clipCaches = &gCache;

		mImpl->texManager.reset(new TextureManager(*device->getVideoDriver()));
		mImpl->meshManager.reset(new MeshManager(*scene));
	}

	IrrlichtComponentSystem::~IrrlichtComponentSystem()
	{
	}

	Component* IrrlichtComponentSystem::create( const string& compName, 
		GameObject& obj, const Properties& param )
	{
		irr_ptr<ISceneManager> smgr = mImpl->smgr;
		RenderComponent* ret = nullptr;

		if (compName == "Mesh")
		{
			IMesh* mesh = nullptr;
			if (const char* path = param.getString("mesh"))
				mesh = mImpl->meshManager->getMesh(path);

			vector3df zero(0, 0, 0), one(1, 1, 1);
			IMeshSceneNode* meshNode = 
				smgr->addMeshSceneNode(mesh, 0, -1, zero, zero, one, true);

			ret = new MeshComponent(compName, obj, meshNode);
		}
		else if (compName == "Light")
		{
			ILightSceneNode* lightNode = smgr->addLightSceneNode();
			ret = new LightComponent(compName, obj, lightNode);
		}
		else if (compName == "Camera")
		{
			irr_ptr<ICameraSceneNode> prevActiveCamera = smgr->getActiveCamera();

			// can't add fps/maya camera
			ICameraSceneNode* cameraNode = smgr->addCameraSceneNode();

			if (param.getBool("active", true) == false)
				smgr->setActiveCamera(prevActiveCamera.get());

			ret = new CameraComponent(compName, obj, cameraNode);
		}
		else if (compName == "AnimatedMesh")
		{
			IAnimatedMesh* mesh = nullptr;
			if (const char* path = param.getString("mesh"))
			{
				// TODO 未做任何检测，因为 Irrlicht 的 IMesh API 设计问题。
				// 将来引入自己的 MeshCache 来解决这一问题。
				mesh = static_cast<IAnimatedMesh*>(mImpl->meshManager->getMesh(path));
			}

			if (mesh)
			{
				for (u32 i = 0; i < mesh->getMeshBufferCount(); i++)
				{
					mesh->getMeshBuffer(i)->setHardwareMappingHint(EHM_STREAM, EBT_INDEX);
					mesh->getMeshBuffer(i)->setHardwareMappingHint(EHM_STREAM, EBT_VERTEX);
				}
			}

			AnimationClips clips;
			if (const char* path = param.getString("clips"))
			{
				string path_s = path;
				if (auto got = StdMap::findValuePtr(*mImpl->clipCaches, path_s))
				{
					clips = *got;
				}
				else
				{
					property_tree::ptree treeData;

					try
					{
						read_json(path_s, treeData);
						clips = AnimationClips::loadFromPtree(treeData);
					}
					catch (property_tree::json_parser_error& error)
					{
						cerr << error.what() << endl;
					}

					// even if the clip data is error, insert it.
					// so we don't need to read the stupid data again
					StdMap::insert(*mImpl->clipCaches, path_s, clips);
				}
			}

			vector3df zero, one(1, 1, 1);
			IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(
				mesh, 0, -1, zero, zero, one, true);

			auto amc = new AnimatedMeshComponent(compName, obj, node);
			amc->createClips(clips);
			ret = amc;
		}
		else if (compName == "SkyDome")
		{
			ITexture* tex = nullptr;
			if (const char* path = param.getString("texture"))
				tex = mImpl->driver->getTexture(path);

			if (!tex) return nullptr;

			int horiRes = param.getInt("horiRes", 16);
			int vertRes = param.getInt("vertRes", 8);
			float texPercent = param.getFloat("texPercent", 0.9f);
			float spherePercent = param.getFloat("spherePercent", 2.f);
			float radius = param.getFloat("radius", 1000.f);

			ISceneNode* node = smgr->addSkyDomeSceneNode(tex, horiRes, vertRes, 
				texPercent, spherePercent, radius);

			ret = new RenderComponent(compName, obj, node);
		}
		else if (compName == "Billboard")
		{
			IBillboardSceneNode* node = smgr->addBillboardSceneNode();
			ret = new BillboardComponent(compName, obj, node);
		}
		else if (compName == "ParticleSystem")
		{
// 			if (const char* path = param.getString("path"))
// 			{
// 				luaT::LuaRef func;
// 				if (auto got = StdMap::findValuePtr(mImpl->particleSystemCreator, string(path)))
// 					func = *got;
// 				else 
// 				{
// 					luaL_dofile(L, path);
// 				}
// 			}
		}

		return ret;
	}

	void IrrlichtComponentSystem::onStart()
	{
	}

	static void syncSceneNode(ISceneNode* node)
	{
		auto iter = node->getChildren().begin();
		while (iter != node->getChildren().end())
		{
			RenderComponent* component = RenderComponent::getComponentFromNode(*iter);
			if (component)
				component->syncWithObject();

			syncSceneNode(*iter);
			++iter;
		}
	}

	void IrrlichtComponentSystem::onUpdate( const Timeline& tl )
	{
		syncSceneNode(mImpl->smgr->getRootSceneNode());
		mImpl->smgr->onAnimate((s32) (tl.getElapsedSeconds()*1000));
		mImpl->smgr->drawAll();
	}

	void IrrlichtComponentSystem::onStop()
	{
	}

	irr::scene::ISceneManager* IrrlichtComponentSystem::getSceneManager()
	{
		return mImpl->smgr.get();
	}

	TextureManager* IrrlichtComponentSystem::getTextureManager()
	{
		return mImpl->texManager.get();
	}

	MeshManager* IrrlichtComponentSystem::getMeshManager()
	{
		return mImpl->meshManager.get();
	}

}}
