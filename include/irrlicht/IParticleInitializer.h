#pragma once
#include "IReferenceCounted.h"

namespace irr { namespace scene 
{
	struct SParticle;
	class IParticleInitializer : public virtual IReferenceCounted
	{
	public:
		virtual void initialize(SParticle& particle) = 0;
	};
}}

