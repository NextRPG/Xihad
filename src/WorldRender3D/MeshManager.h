#pragma once
#include <Engine\irr_ptr.h>

namespace irr { namespace scene
{
	class IMesh;
	class ISceneManager;
}}

namespace xihad { namespace render3d
{
	class MeshManager
	{
	public:
		typedef const char* MeshID;
		typedef irr::scene::IMesh Mesh;

	public:
		MeshManager(irr::scene::ISceneManager& smgr);
		~MeshManager();

		Mesh* getMesh(MeshID id);
		void addMesh(MeshID id, Mesh* mesh);
		void renameMesh(MeshID newId, Mesh*);
		void removeMesh(Mesh*);
		void clearUnusedMeshes();

	private:
		irr_ptr<irr::scene::ISceneManager> smgr;
	};
}}

