// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_PARTICLE_GRAVITY_AFFECTOR_H_INCLUDED__
#define __I_PARTICLE_GRAVITY_AFFECTOR_H_INCLUDED__

#include "IParticleAffector.h"

namespace irr
{
namespace scene
{

//! A particle affector which applies gravity to particles.
class IParticleForceAffector : public IParticleAffector
{
public:
	//! Set the direction and force of gravity in all 3 dimensions.
	virtual void setGravity( const core::vector3df& gravity ) = 0;

	virtual void setForce(const core::vector3df& force) = 0;

	//! Get the direction and force of gravity.
	virtual const core::vector3df& getGravity() const = 0;

	virtual const core::vector3df& getForce() const = 0;
};

} // end namespace scene
} // end namespace irr


#endif // __I_PARTICLE_GRAVITY_AFFECTOR_H_INCLUDED__

