#pragma once
#include "IParticleRenderer.h"

namespace irr { namespace scene 
{
	class IPointParticleRenderer : public IParticleRenderer
	{
	public:
		virtual void setPointSize(float) = 0;
	};
}}

