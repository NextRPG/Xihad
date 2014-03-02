#pragma once
#include "IParticleRenderer.h"

namespace irr { namespace scene
{
	class IParticlePointRenderer : public IParticleRenderer
	{
	public:
		IParticlePointRenderer(video::IVideoDriver* driver, core::array<SParticle>* source) :
			IParticleRenderer(driver, source)
		{
		}

		virtual void setPointSize(f32) = 0;
		virtual f32 getPointSize() const = 0;
	};
}}

