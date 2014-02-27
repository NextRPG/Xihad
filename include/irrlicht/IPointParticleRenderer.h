#pragma once
#include "IParticleRenderer.h"

namespace irr { namespace scene
{
	class IPointParticleRenderer : public IParticleRenderer
	{
	public:
		IPointParticleRenderer(video::IVideoDriver* driver, core::array<SParticle>* source) :
			IParticleRenderer(driver, source)
		{
		}

		virtual void setPointSize(f32) = 0;
		virtual f32 getPointSize() const = 0;
	};
}}

