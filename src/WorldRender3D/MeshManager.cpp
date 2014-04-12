#include "MeshManager.h"
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IMeshCache.h>
#include <irrlicht/EHardwareBufferFlags.h>

using namespace irr;
using namespace scene;
namespace xihad { namespace render3d
{
	MeshManager::MeshManager( irr::scene::ISceneManager& smgr ) :
		smgr(&smgr) { }

	MeshManager::~MeshManager() { }

	MeshManager::Mesh* MeshManager::getMesh( MeshID id )
	{
		Mesh* mesh = smgr->getMesh(id);

		/// TODO
		if (mesh)
		{
			for (u32 i = 0; i < mesh->getMeshBufferCount(); i++)
			{
				mesh->getMeshBuffer(i)->setHardwareMappingHint(EHM_STREAM, EBT_INDEX);
				mesh->getMeshBuffer(i)->setHardwareMappingHint(EHM_STREAM, EBT_VERTEX);
			}
		}

		return mesh;
	}

	void MeshManager::addMesh( MeshID id, Mesh* mesh )
	{
		if (mesh)
		{
			smgr->getMeshCache()->addMesh(id, 
				static_cast<scene::IAnimatedMesh*>(mesh));
		}
	}

	void MeshManager::renameMesh( MeshID newId, Mesh* mesh )
	{
		smgr->getMeshCache()->renameMesh(mesh, newId);
	}

	void MeshManager::removeMesh( Mesh* mesh )
	{
		smgr->getMeshCache()->removeMesh(mesh);
	}

	void MeshManager::clearUnusedMeshes()
	{
		smgr->getMeshCache()->clearUnusedMeshes();
	}

}}

