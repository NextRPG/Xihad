#include "CameraComponent.h"
#include "irrlicht/ICameraSceneNode.h"
#include "irrlicht/ISceneManager.h"
#include "Engine/GameObject.h"

using namespace irr::scene;
using namespace xihad::ngn;
namespace xihad { namespace render3d
{
	CameraComponent::CameraComponent( const std::string& name, 
		ngn::GameObject& host, irr::scene::ICameraSceneNode* node ) :
		RenderComponent(name, host, node)
	{
	}

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

	const irr::core::vector3df& CameraComponent::getTarget() const
	{
		return getNode()->getTarget();
	}

	void CameraComponent::setUpVector( const irr::core::vector3df& pos )
	{
		getNode()->setUpVector(pos);
	}

	const irr::core::vector3df& CameraComponent::getUpVector() const
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

	const irr::scene::SViewFrustum* CameraComponent::getViewFrustum() const
	{
		return getNode()->getViewFrustum();
	}

	bool CameraComponent::isOrthogonal() const
	{
		return getNode()->isOrthogonal();
	}

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

	irr::scene::ICameraSceneNode * CameraComponent::getNode() const
	{
		return (irr::scene::ICameraSceneNode*) RenderComponent::getNode();
	}

}}
