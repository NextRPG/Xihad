#include "CWrappedMeshInitializer.h"
#include <irrlicht/IParticleMeshInitializer.h>
#include <irrlicht/IParticleAnimatedMeshInitializer.h>
#include <iostream>
#include <irrlicht/IAnimatedMeshSceneNode.h>

namespace xihad { namespace particle
{
	void CWrappedMeshInitializer::setMesh( scene::IMesh* mesh )
	{
		MeshIniter->setMesh(mesh);
	}
}}

