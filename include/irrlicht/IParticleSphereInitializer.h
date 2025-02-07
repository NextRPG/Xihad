// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_PARTICLE_SPHERE_EMITTER_H_INCLUDED__
#define __I_PARTICLE_SPHERE_EMITTER_H_INCLUDED__

#include "IParticleGeometricInitializer.h"
#include "vector3d.h"

namespace irr
{
namespace scene
{

//! A particle emitter which emits from a spherical space.
class IParticleSphereInitializer : public IParticleGeometricInitializer
{
public:
	//! Set the center of the sphere for particle emissions
	virtual void setCenter( const core::vector3df& center ) = 0;

	//! Set the radius of the sphere for particle emissions
	virtual void setRadius( f32 radius ) = 0;

	//! Get the center of the sphere for particle emissions
	virtual const core::vector3df& getCenter() const = 0;

	//! Get the radius of the sphere for particle emissions
	virtual f32 getRadius() const = 0;

};

} // end namespace scene
} // end namespace irr


#endif

