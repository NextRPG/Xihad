// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_PARTICLE_MESH_EMITTER_H_INCLUDED__
#define __I_PARTICLE_MESH_EMITTER_H_INCLUDED__

#include "IParticleGeometricInitializer.h"
#include "IMesh.h"

namespace irr
{
namespace scene
{

//! A particle emitter which emits from vertices of a mesh
class IParticleMeshInitializer : public virtual IParticleGeometricInitializer
{
public:
	//! Set Mesh to emit particles from
	virtual void setMesh( IMesh* mesh ) = 0;

	//! Get Mesh we're emitting particles from
	virtual const IMesh* getMesh() const = 0;
};

} // end namespace scene
} // end namespace irr


#endif

