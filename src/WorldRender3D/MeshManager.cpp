#include "MeshManager.h"
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IMeshCache.h>

using namespace irr;
namespace xihad { namespace render3d
{
	MeshManager::MeshManager( irr::scene::ISceneManager& smgr ) :
		smgr(&smgr) { }

	MeshManager::~MeshManager() { }

	MeshManager::Mesh* MeshManager::getMesh( MeshID id )
	{
		return smgr->getMesh(id);
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

