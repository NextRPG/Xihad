// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_PARTICLE_MESH_EMITTER_H_INCLUDED__
#define __I_PARTICLE_MESH_EMITTER_H_INCLUDED__

#include "IParticleInitializer.h"
#include "IMesh.h"

namespace irr
{
namespace scene
{

//! A particle emitter which emits from vertices of a mesh
class IParticleMeshInitializer : public IParticleInitializer
{
public:
	//! Set Mesh to emit particles from
	virtual void setMesh( IMesh* mesh ) = 0;

	virtual void setMinNormalDirectionSize( f32 minSize ) = 0;

	virtual void setMaxNormalDirectionSize( f32 maxSize ) = 0;

	//! Get Mesh we're emitting particles from
	virtual const IMesh* getMesh() const = 0;

	virtual f32 getMinNormalDirectionSize() const = 0;

	virtual f32 getMaxNormalDirectionSize() const = 0;
};

} // end namespace scene
} // end namespace irr


#endif

