#pragma once
#include "IReferenceCounted.h"
#include "IVideoDriver.h"
#include "matrix4.h"
#include "aabbox3d.h"

namespace irr { namespace video 
{
	class SMaterial;
}}

namespace irr { namespace scene
{
	class ICameraSceneNode;
	struct SParticle;
	class IParticleRenderer : public virtual IReferenceCounted
	{
	public:
		IParticleRenderer(video::IVideoDriver* driver, core::array<SParticle>* source) :
			Driver(driver), ParticleSource(source) 
		{
			Driver->grab();
		}

		virtual ~IParticleRenderer()
		{
			Driver->drop();
		}

		virtual void render(ICameraSceneNode* camera, const core::matrix4& modelSpace, bool bboxVisible) = 0;

		virtual void updateBoundingBox() = 0;

		virtual void onParticleSwapped(u32 index) = 0;

		virtual core::aabbox3df& getBoundingBox() = 0;

		virtual video::SMaterial& getMaterial() = 0;

		void setParticleSource(core::array<SParticle>* particles)
		{
			ParticleSource = particles;
		}

		core::array<SParticle>* getParticleSource() const
		{
			return ParticleSource;
		}

	protected:
		video::IVideoDriver* const Driver;

	private:
		core::array<SParticle>* ParticleSource;
	};
}}

