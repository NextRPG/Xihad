#pragma once
#include "RenderComponent.h"
#include "Engine/matrix.h"
#include "Engine/vector3d.h"

namespace irr 
{
	struct SEvent;

	namespace scene
	{
		struct SViewFrustum;
		class ICameraSceneNode;
	}
}

namespace xihad { namespace render3d
{
	class CameraComponent : public RenderComponent
	{
	public:
		DEFINE_VISITABLE

		CameraComponent(const std::string& name, ngn::GameObject& host, irr::scene::ICameraSceneNode* node);
		
		void activate();

		bool isActivating() const;

		void setProjectionMatrix(const ngn::Matrix& projection, bool isOrthogonal=false);

		//! Gets the current projection matrix of the camera.
		/** \return The current projection matrix of the camera. */
		const ngn::Matrix& getProjectionMatrix() const;

		//! Gets the current view matrix of the camera.
		/** \return The current view matrix of the camera. */
		const ngn::Matrix& getViewMatrix() const;

		//! Sets a custom view matrix affector.
		/** The matrix passed here, will be multiplied with the view
		matrix when it gets updated. This allows for custom camera
		setups like, for example, a reflection camera.
		\param affector The affector matrix. */
		void setViewMatrixAffector(const ngn::Matrix& affector);

		//! Get the custom view matrix affector.
		/** \return The affector matrix. */
		const ngn::Matrix& getViewMatrixAffector() const;

		//! Sets the look at target of the camera
		/** If the camera's target and rotation are bound ( @see
		bindTargetAndRotation() ) then calling this will also change
		the camera's scene node rotation to match the target.
		Note that setTarget uses the current absolute position 
		internally, so if you changed setPosition since last rendering you must
		call updateAbsolutePosition before using this function.
		\param pos Look at target of the camera, in world co-ordinates. */
		void setTarget(const ngn::vector3df& pos);

		//! Gets the current look at target of the camera
		/** \return The current look at target of the camera, in world co-ordinates */
		const irr::core::vector3df& getTarget() const;

		//! Sets the up vector of the camera.
		/** \param pos: New upvector of the camera. */
		void setUpVector(const irr::core::vector3df& pos);

		//! Gets the up vector of the camera.
		/** \return The up vector of the camera, in world space. */
		const irr::core::vector3df& getUpVector() const;

		//! Gets the value of the near plane of the camera.
		/** \return The value of the near plane of the camera. */
		float getNearValue() const;

		//! Gets the value of the far plane of the camera.
		/** \return The value of the far plane of the camera. */
		float getFarValue() const;

		//! Gets the aspect ratio of the camera.
		/** \return The aspect ratio of the camera. */
		float getAspectRatio() const;

		//! Gets the field of view of the camera.
		/** \return The field of view of the camera in radians. */
		float getFOV() const;

		//! Sets the value of the near clipping plane. (default: 1.0f)
		/** \param zn: New z near value. */
		void setNearValue(float zn);

		//! Sets the value of the far clipping plane (default: 2000.0f)
		/** \param zf: New z far value. */
		void setFarValue(float zf);

		//! Sets the aspect ratio (default: 4.0f / 3.0f)
		/** \param aspect: New aspect ratio. */
		void setAspectRatio(float aspect);

		//! Sets the field of view (Default: PI / 2.5f)
		/** \param fovy: New field of view in radians. */
		void setFOV(float fovy);

		//! Get the view frustum.
		/** Needed sometimes by bspTree or LOD render nodes.
		\return The current view frustum. */
		const irr::scene::SViewFrustum* getViewFrustum() const;

		//! Checks if a camera is orthogonal.
		bool isOrthogonal() const;

	protected:
		virtual void onStop();

		irr::scene::ICameraSceneNode * getNode() const;
	};
}}

