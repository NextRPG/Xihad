// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_PARTICLE_AFFECTOR_H_INCLUDED__
#define __I_PARTICLE_AFFECTOR_H_INCLUDED__

#include "IReferenceCounted.h"
#include "SParticle.h"

namespace irr
{
namespace scene
{

//! A particle affector modifies particles.
class IParticleAffector : public virtual IReferenceCounted
{
public:

	//! constructor
	IParticleAffector() {}

	//! Affects an array of particles.
	/** \param timediff Elapsed milliseconds since last affect
	\param particle To affect particle. */
	void affect(f32 timediff, f32 d, f32 dd, SParticle& particle) 
	{
		doAffect(timediff, d, dd, particle);
	}

protected:
	virtual void doAffect(f32 timediff, f32 d, f32 dd, SParticle& particle) = 0;

};

} // end namespace scene
} // end namespace irr


#endif

