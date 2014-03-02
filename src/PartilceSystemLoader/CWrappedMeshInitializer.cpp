#include "CWrappedMeshInitializer.h"
#include <irrlicht/IParticleMeshInitializer.h>
#include <irrlicht/IParticleAnimatedMeshInitializer.h>

using namespace irr::scene;
namespace xihad { namespace particle
{
	void CWrappedMeshInitializer::setMesh( const char* meshDesc )
	{
		if (auto miniter = dynamic_cast<IParticleMeshInitializer*>(MeshIniter.get()))
		{
			// miniter->setMesh(..);
		}
	}

	void CWrappedMeshInitializer::setAnimatedMesh( const char* animatedMeshDesc )
	{
		if (auto ainiter = dynamic_cast<IParticleAnimatedMeshInitializer*>(MeshIniter.get()))
		{
			// ainiter->setAnimatedMesh(..);
		}
	}

}}

