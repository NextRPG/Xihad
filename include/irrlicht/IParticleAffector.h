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
	IParticleAffector() : Enabled(true) {}

	//! Affects an array of particles.
	/** \param timediff Elapsed milliseconds since last affect
	\param particle To affect particle. */
	void affect(f32 timediff, f32 d, SParticle& particle) 
	{
		if (Enabled)
			doAffect(timediff, d, particle);
	}

	//! Sets whether or not the affector is currently enabled.
	virtual void setEnabled(bool enabled) { Enabled = enabled; }

	//! Gets whether or not the affector is currently enabled.
	virtual bool getEnabled() const { return Enabled; }

protected:
	virtual void doAffect(f32 timediff, f32 d, SParticle& particle) = 0;

private:
	bool Enabled;
};

} // end namespace scene
} // end namespace irr


#endif

