#include "CameraComponent.h"
#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>
#include <CppBase/xassert.h>
#include <iostream>

using namespace std;
using namespace irr::video;
using namespace irr::scene;
using namespace xihad::ngn;
namespace xihad { namespace render3d
{
	CameraComponent::RenderTarget::RenderTarget(ITexture& texture) :
		texture(&texture), renderToTexture(true) 
	{
	}

	CameraComponent::RenderTarget::RenderTarget(E_RENDER_TARGET target) :
		target(target), renderToTexture(false) 
	{
		xassert(target != ERT_RENDER_TEXTURE);
	}

	void CameraComponent::setRendererTarget( RenderTarget renderTarget )
	{
		if (renderTarget.renderToTexture)
		{
			this->renderTarget = ERT_RENDER_TEXTURE;
			this->renderTexture.reset(0);
		}
		else
		{
			if (renderTarget.target == ERT_MULTI_RENDER_TEXTURES)
			{
				cerr << "Not support Multi render texutres yet"	<<  endl;
				return;
			}

			this->renderTarget = renderTarget.target;
			xassert(renderTarget.texture);
			this->renderTexture.reset(renderTarget.texture);
		}
	}

	CameraComponent::RenderTarget CameraComponent::getRenderTarget()
	{
		if (renderTarget != ERT_RENDER_TEXTURE)
		{
			return RenderTarget(renderTarget);
		}
		else 
		{
			xassert(renderTexture.get() != nullptr);
			return RenderTarget(*renderTexture.get());
		}
	}


///////////////////////////////////////////////////////////////////////////////////
	CameraComponent::CameraComponent( const std::string& name, 
		ngn::GameObject& host, ICameraSceneNode* node ) :
		RenderComponent(name, host, node), 
		renderTarget(ERT_FRAME_BUFFER), renderTexture(0) { }

	void CameraComponent::setProjectionMatrix( const ngn::Matrix& projection, bool isOrthogonal/*=false*/ )
	{
		getNode()->setProjectionMatrix(projection, isOrthogonal);
	}

	const ngn::Matrix& CameraComponent::getProjectionMatrix() const
	{
		return getNode()->getProjectionMatrix();
	}

	const ngn::Matrix& CameraComponent::getViewMatrix() const
	{
		return getNode()->getViewMatrix();
	}

	void CameraComponent::setViewMatrixAffector( const ngn::Matrix& affector )
	{
		getNode()->setViewMatrixAffector(affector);
	}

	const ngn::Matrix& CameraComponent::getViewMatrixAffector() const
	{
		return getNode()->getViewMatrixAffector();
	}

	void CameraComponent::setTarget( const vector3df& worldTarget )
	{
		getNode()->setTarget(worldTarget);
	}

	const vector3df& CameraComponent::getTarget() const
	{
		return getNode()->getTarget();
	}

	void CameraComponent::setUpVector( const vector3df& pos )
	{
		getNode()->setUpVector(pos);
	}

	const vector3df& CameraComponent::getUpVector() const
	{
		return getNode()->getUpVector();
	}

	float CameraComponent::getNearValue() const
	{
		return getNode()->getNearValue();
	}

	float CameraComponent::getFarValue() const
	{
		return getNode()->getFarValue();
	}

	float CameraComponent::getAspectRatio() const
	{
		return getNode()->getAspectRatio();
	}

	float CameraComponent::getFOV() const
	{
		return getNode()->getFOV();
	}

	void CameraComponent::setNearValue( float zn )
	{
		getNode()->setNearValue(zn);
	}

	void CameraComponent::setFarValue( float zf )
	{
		getNode()->setFarValue(zf);
	}

	void CameraComponent::setAspectRatio( float aspect )
	{
		getNode()->setAspectRatio(aspect);
	}

	void CameraComponent::setFOV( float fovy )
	{
		getNode()->setFOV(fovy);
	}

	const SViewFrustum* CameraComponent::getViewFrustum() const
	{
		return getNode()->getViewFrustum();
	}

	bool CameraComponent::isOrthogonal() const
	{
		return getNode()->isOrthogonal();
	}

	/// Add self into scene manager's render queue
	void CameraComponent::activate()
	{
		ISceneManager* smgr = getNode()->getSceneManager();
		smgr->setActiveCamera(getNode());
	}

	bool CameraComponent::isActivating() const
	{
		ICameraSceneNode const * n = getNode();
		ISceneManager* smgr = n->getSceneManager();
		return smgr->getActiveCamera() == n;
	}

	void CameraComponent::onStop()
	{
		RenderComponent::onStop();
		if (isActivating())
			getNode()->getSceneManager()->setActiveCamera(nullptr);
	}

	ICameraSceneNode * CameraComponent::getNode() const
	{
		return (ICameraSceneNode*) RenderComponent::getNode();
	}
}}
