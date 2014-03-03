#include "CWrappedMeshInitializer.h"
#include <irrlicht/IParticleMeshInitializer.h>
#include <irrlicht/IParticleAnimatedMeshInitializer.h>
#include <iostream>
#include <irrlicht/IAnimatedMeshSceneNode.h>

using namespace irr::scene;
namespace xihad { namespace particle
{
	void CWrappedMeshInitializer::setMesh( irr::scene::IMesh* mesh )
	{
		MeshIniter->setMesh(mesh);
	}

}}

