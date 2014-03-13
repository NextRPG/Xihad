#include "IrrlichtComponentSystem.h"
#include <map>							// |
#include <unordered_map>				// |
#include <Engine\BiAssociateMap.h>		// |
#include "CameraRenderTarget.h"			// +----> includes for CameraRenderer
#include <Engine\irr_ptr.h>
#include <irrlicht\ISceneManager.h>
#include "TextureManager.h"
#include "MeshManager.h"
#include <irrlicht\IrrlichtDevice.h>	
/* All above headers used in field */

/* All below headers used in implemention */
#include <CppBase\xassert.h>
#include <Engine\Timeline.h>
#include "MeshComponent.h"				// |
#include "LightComponent.h"				// |
#include "CameraComponent.h"			// |
#include "AnimatedMeshComponent.h"		// |
#include "BillboardComponent.h"			// |
#include "ParticleSystemComponent.h"	// +-----> includes for all Components

using namespace std;
using namespace boost;
namespace xihad { namespace render3d
{
	using namespace video;
	using namespace ngn;

	typedef std::multimap<CameraRenderTarget, CameraComponent*> RenderTarget2Cam;
	typedef std::unordered_map<CameraComponent*, RenderTarget2Cam::iterator> Cam2RenderTarget;
	typedef BiAssociateMap<CameraRenderTarget, CameraComponent*, RenderTarget2Cam, Cam2RenderTarget> CameraRenderer;
	struct IrrlichtComponentSystem::impl
	{
		irr_ptr<ISceneManager> smgr;
		AnimationClipsCache* clipsCache;
		boost::scoped_ptr<TextureManager> texManager;
		boost::scoped_ptr<MeshManager> meshManager;

		CameraRenderer cameraRenderers;
	};

	IrrlichtComponentSystem::IrrlichtComponentSystem( 
		IrrlichtDevice* device, ISceneManager* scene, 
		const InheritanceTree& tree, AnimationClipsCache* gCache) :
	BaseComponentSystem(tree), mImpl(new impl)
	{
		xassert(device);
		xassert(scene);

		mImpl->smgr = scene;
		mImpl->clipsCache = gCache;

		mImpl->texManager.reset(new TextureManager(*device->getVideoDriver()));
		mImpl->meshManager.reset(new MeshManager(*scene));
	}

	IrrlichtComponentSystem::~IrrlichtComponentSystem() {}

	Component* IrrlichtComponentSystem::create(const string& compName, GameObject& obj, const Properties& param )
	{
		ISceneManager* smgr = mImpl->smgr.get();
		RenderComponent* ret = nullptr;

		if (compName == "Mesh")
			ret = MeshComponent::create(compName, obj, param, smgr, mImpl->meshManager.get());
		else if (compName == "AnimatedMesh")
			ret = AnimatedMeshComponent::create(compName, obj, param, smgr, mImpl->clipsCache, mImpl->meshManager.get());
		else if (compName == "Billboard")
			ret = BillboardComponent::create(compName, obj, param, smgr);
		else if (compName == "ParticleSystem")
			ret = 0;	// TODO
		else if (compName == "Light")
			ret = LightComponent::create(compName, obj, param, smgr);
		else if (compName == "Camera")
			ret = CameraComponent::create(compName, obj, param, smgr, this);
		else if (compName == "SkyDome")
			ret = RenderComponent::createSkyDomeComponent(compName, obj, param, smgr, mImpl->texManager.get());
		
		return ret;
	}

	void IrrlichtComponentSystem::onStart()
	{
	}

	static void syncSceneNode(ISceneNode* node)
	{
		auto iter = node->getChildren().begin();
		auto stop = node->getChildren().end();
		while (iter != stop)
		{
			RenderComponent* component = RenderComponent::getComponentFromNode(*iter);
			if (component)
				component->syncWithObject();

			// syncSceneNode(*iter);
			++iter;
		}
	}

	void IrrlichtComponentSystem::onUpdate( const Timeline& tl )
	{
		syncSceneNode(mImpl->smgr->getRootSceneNode());
		mImpl->smgr->onAnimate((s32) (tl.getElapsedSeconds()*1000));

		video::IVideoDriver* driver = mImpl->smgr->getVideoDriver();

		int target = -1;
		CameraRenderTarget::RenderTexture* texture = 0; 
		for (auto& kv : mImpl->cameraRenderers.pairs())
		{
			CameraComponent* camera = kv.second;

			if (!camera->isActive()) continue;

			const CameraRenderTarget& rt = kv.first;
			if (rt.getType() != target && rt.getType() != ERT_RENDER_TEXTURE)
			{
				if (rt.getType() == ERT_MULTI_RENDER_TEXTURES)
				{
					cerr << "Multi render textures are not supported yet" << endl;
					continue;
				}

				target = kv.first.getType();
				
				if (!driver->setRenderTarget(target))
					cerr << "RenderTarget set failed" << endl;
			}
			else if (rt.getRenderTexture() != 0 && rt.getRenderTexture() != texture)
			{
				target = ERT_RENDER_TEXTURE;
				texture = rt.getRenderTexture();

				if (!driver->setRenderTarget(texture))
					cerr << "RenderTexture set failed" << endl;
			}
			assert(target != -1);

			core::recti viewport = camera->getAbsoluteViewport(driver->getCurrentRenderTargetSize());
			driver->setViewPort(viewport);
			mImpl->smgr->setActiveCamera(camera->getNode());
			mImpl->smgr->drawAll();
		}

		// TODO Add interface to integrate other rendering module with MRT.
		// Now we have to switch back to frame buffer to let gui or other modules draw on window default.
		driver->setRenderTarget(video::ERT_FRAME_BUFFER, false, false);
	}

	void IrrlichtComponentSystem::onStop()
	{
		mImpl->smgr.reset(0);
	}

	ISceneManager* IrrlichtComponentSystem::getSceneManager()
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

	void IrrlichtComponentSystem::addCamera( const CameraRenderTarget& rt, CameraComponent * camcom )
	{
		mImpl->cameraRenderers.add(rt, camcom);
	}

	void IrrlichtComponentSystem::removeCamera( CameraComponent* camcom)
	{
		mImpl->cameraRenderers.remove(camcom);
	}

}}
