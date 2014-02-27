// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_PARTICLE_EMITTER_H_INCLUDED__
#define __I_PARTICLE_EMITTER_H_INCLUDED__

#include "IReferenceCounted.h"
#include "SParticle.h"

namespace irr
{
namespace scene
{

class IParticleInitializer;
//! A particle emitter for using with particle systems.
/** A Particle emitter emitts new particles into a particle system.
*/
class IParticleEmitter : public virtual IReferenceCounted
{
public:
	//! Prepares an array with new particles to emitt into the system
	/** \param now Current time.
	\param timeSinceLastCall Time elapsed since last call, in milliseconds.
	\param outArray Pointer which will point to the array with the new
	particles to add into the system.
	\return Amount of new particles in the array. Can be 0. */
	virtual s32 emitt(f32 now, f32 timeSinceLastCall, SParticle*& outArray) = 0;

	virtual void setParticleInitializer(IParticleInitializer*) = 0;

	virtual IParticleInitializer* getParticleInitializer() const = 0;

	//! Set minimum number of particles the emitter emits per second
	virtual void setMinParticlesPerSecond( u32 minPPS ) = 0;

	//! Set maximum number of particles the emitter emits per second
	virtual void setMaxParticlesPerSecond( u32 maxPPS ) = 0;

	//! Gets the minimum number of particles the emitter emits per second
	virtual u32 getMinParticlesPerSecond() const = 0;

	//! Gets the maximum number of particles the emitter emits per second
	virtual u32 getMaxParticlesPerSecond() const = 0;

};

typedef IParticleEmitter IParticlePointEmitter;

} // end namespace scene
} // end namespace irr


#endif

