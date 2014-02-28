// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_PARTICLE_ANIMATED_MESH_SCENE_NODE_EMITTER_H_INCLUDED__
#define __I_PARTICLE_ANIMATED_MESH_SCENE_NODE_EMITTER_H_INCLUDED__

#include "IParticleInitializer.h"
#include "IAnimatedMeshSceneNode.h"

namespace irr
{
namespace scene
{

//! A particle emitter which emits particles from mesh vertices.
class IParticleAnimatedMeshInitializer : public IParticleInitializer
{
public:

	//! Set Mesh to emit particles from
	virtual void setAnimatedMeshSceneNode( IAnimatedMeshSceneNode* node ) = 0;

	virtual void setMinNormalDirectionSize( f32 minSize ) = 0;

	virtual void setMaxNormalDirectionSize( f32 maxSize ) = 0;

	//! Get mesh we're emitting particles from
	virtual const IAnimatedMeshSceneNode* getAnimatedMeshSceneNode() const = 0;

	virtual f32 getMinNormalDirectionSize() const = 0;

	virtual f32 getMaxNormalDirectionSize() const = 0;

};

} // end namespace scene
} // end namespace irr


#endif // __I_PARTICLE_ANIMATED_MESH_SCENE_NODE_EMITTER_H_INCLUDED__

