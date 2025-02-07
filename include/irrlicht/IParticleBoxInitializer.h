// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_PARTICLE_BOX_EMITTER_H_INCLUDED__
#define __I_PARTICLE_BOX_EMITTER_H_INCLUDED__

#include "IParticleGeometricInitializer.h"
#include "aabbox3d.h"

namespace irr
{
namespace scene
{

//! A particle emitter which emits particles from a box shaped space
class IParticleBoxInitializer : public IParticleGeometricInitializer
{
public:
	//! Set the box shape
	virtual void setBox( const core::aabbox3df& box ) = 0;

	//! Get the box shape set
	virtual const core::aabbox3df& getBox() const = 0;

};

} // end namespace scene
} // end namespace irr


#endif

