#include "CameraComponent.h"
#include <Engine/GameObject.h>
#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>
#include "IrrlichtComponentSystem.h"

using namespace std;
namespace xihad { namespace render3d
{
	using namespace ngn;
	using namespace video;

	CameraComponent::CameraComponent( const std::string& name, 
		ngn::GameObject& host, ICameraSceneNode* node, IrrlichtComponentSystem* sys) :
		RenderComponent(name, host, node), active(true), irrlichtSystem(sys),
		viewport(0, 0, 1, 1), fixedTarget(true)
	{
		sys->addCamera(renderTarget, this);
	}

	CameraComponent* CameraComponent::create(
		const std::string& compName, ngn::GameObject& obj, const ngn::Properties& param,
		ISceneManager* smgr, IrrlichtComponentSystem* sys)
	{
		ICameraSceneNode* cameraNode = smgr->addCameraSceneNode();
		CameraComponent* camcom = new CameraComponent(compName, obj, cameraNode, sys);

		if (param.getBool("active", true) == false)
			camcom->setActive(false);

		return camcom;
	}

	void CameraComponent::setRendererTarget( CameraRenderTarget renderTarget )
	{
		if (this->renderTarget != renderTarget)
		{
			this->renderTarget = renderTarget;
			irrlichtSystem->addCamera(renderTarget, this);
		}
	}

	core::recti CameraComponent::getAbsoluteViewport( const core::dimension2du& size ) const
	{
		return core::recti(
			(int) (viewport.UpperLeftCorner.X  * size.Width), 
			(int) (viewport.UpperLeftCorner.Y  * size.Height),
			(int) (viewport.LowerRightCorner.X * size.Width),
			(int) (viewport.LowerRightCorner.Y * size.Height));
	}

	/// Add self into scene manager's render queue
	void CameraComponent::setActive(bool active)
	{
		if (this->active == active) return;

		this->active = active;

		ISceneManager* smgr = getNode()->getSceneManager();
		smgr->setActiveCamera(getNode());
	}

	bool CameraComponent::isActive() const
	{
		return active;
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
		if (!fixedTarget)
			setLookDirection(worldTarget - getTranslate());
		else
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

	void CameraComponent::onStop()
	{
		irrlichtSystem->removeCamera(this);
		RenderComponent::onStop();
	}

	ICameraSceneNode * CameraComponent::getNode() const
	{
		return (ICameraSceneNode*) RenderComponent::getNode();
	}

	void CameraComponent::syncWithObject()
	{
		RenderComponent::syncWithObject();

		if (!fixedTarget)
		{
			vector3df pos = getTranslate();
			setTarget(pos + getLookDirection());
		}
	}

	void CameraComponent::setTargetFixed( bool fixedTarget )
	{
		if (!fixedTarget && this->fixedTarget)
			lookDir = getLookDirection();

		this->fixedTarget = fixedTarget;
	}

	bool CameraComponent::isTargetFixed() const
	{
		return this->fixedTarget;
	}

	core::vector3df CameraComponent::getLookDirection() const
	{
		if (!fixedTarget) return lookDir;

		// fix target
		return getTarget() - getTranslate();
	}

	void CameraComponent::setLookDirection( const vector3df& lookdir )
	{
		if (fixedTarget)
			setTarget(getTranslate() + lookdir);
		else
			this->lookDir = lookdir;
	}

	const core::vector3df& CameraComponent::getTranslate() const
	{
		return getHostObject()->getWorldTransform().getTranslation();
	}

}}
