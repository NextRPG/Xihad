#pragma once
#include "IParticleInitializer.h"
#include "vector3d.h"

namespace irr { namespace scene 
{
	class IParticleDirectionInitializer : public IParticleInitializer
	{
	public:
		virtual void setDirection(const core::vector3df&) = 0;
		virtual const core::vector3df& getDirection() const = 0;

		virtual void setMaxAngleDegree(f32) = 0;
		virtual f32 getMaxAngleDegree() const = 0;
	};
}}
