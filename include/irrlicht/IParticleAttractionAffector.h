// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_PARTICLE_ATTRACTION_AFFECTOR_H_INCLUDED__
#define __I_PARTICLE_ATTRACTION_AFFECTOR_H_INCLUDED__

#include "IParticleAffector.h"

namespace irr
{
namespace scene
{

//! A particle affector which attracts or detracts particles.
class IParticleAttractionAffector : public IParticleAffector
{
public:

	//! Set the point that particles will attract to
	virtual void setPoint( const core::vector3df& point ) = 0;

	virtual void setMass(f32 mass) = 0;

	//! Get the point that particles are attracted to
	virtual const core::vector3df& getPoint() const = 0;

	virtual f32 getMass() const = 0;
};

} // end namespace scene
} // end namespace irr


#endif // __I_PARTICLE_ATTRACTION_AFFECTOR_H_INCLUDED__

