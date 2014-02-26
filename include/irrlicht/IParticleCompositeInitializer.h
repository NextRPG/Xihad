#pragma once
#include "IParticleInitializer.h"

namespace irr { namespace scene 
{
	class IParticleCompositeInitializer : public IParticleInitializer
	{
	public:
		virtual void clear() = 0;

		virtual void pushBack(IParticleInitializer*) = 0;
		virtual void popBack() = 0;

		virtual void pushFront(IParticleInitializer*) = 0;
		virtual void popFront() = 0;
	};
}}

