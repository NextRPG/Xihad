#include <luaT/luaT.h>
#include <irrlicht/IMesh.h>
#include <irrlicht/ITexture.h>
#include <irrlicht/IAnimatedMesh.h>
#include "../MeshManager.h"
#include "../TextureManager.h"

using namespace xihad::render3d;
using namespace luaT;
using namespace irr;
using namespace scene;
namespace xihad { namespace render3d
{
	luaT_static void takeMesh(MeshManager* mm, const char* id, IMesh* mesh)
	{
		mm->addMesh(id, mesh);
		mesh->drop();
	}}

	int luaopen_ResourceManager(lua_State* L)
	{
		luaT_defRegsBgn(tex)
			luaT_cnnamefunc(intrusive_ptr_add_ref<TextureManager::Texture>, grab),
			luaT_cnnamefunc(intrusive_ptr_release<TextureManager::Texture>, drop),
		luaT_defRegsEnd
		MetatableFactory<TextureManager::Texture>::create(L, tex);

		luaT_defRegsBgn(texm)
			luaT_mnamedfunc(TextureManager, getTexture),
			luaT_mnamedfunc(TextureManager, renameTexture),
			luaT_mnamedfunc(TextureManager, removeTexture),
			// luaT_mnamedfunc(TextureManager, addRenderTargetTexture),
		luaT_defRegsEnd
		MetatableFactory<TextureManager>::create(L, texm);

		/////////////////////////////////////////////////////////////
		luaT_defRegsBgn(msh)
			luaT_cnnamefunc(intrusive_ptr_add_ref<MeshManager::Mesh>, grab),
			luaT_cnnamefunc(intrusive_ptr_release<MeshManager::Mesh>, drop),
		luaT_defRegsEnd
		MetatableFactory<MeshManager::Mesh>::create(L, msh);

		MetatableFactory<IAnimatedMesh, IMesh>::createNull(L);

		luaT_defRegsBgn(mshm)
			luaT_cnamedfunc(takeMesh),
			luaT_mnamedfunc(MeshManager, getMesh),
			luaT_mnamedfunc(MeshManager, addMesh),
			luaT_mnamedfunc(MeshManager, renameMesh),
			luaT_mnamedfunc(MeshManager, removeMesh),
			luaT_mnamedfunc(MeshManager, clearUnusedMeshes),
		luaT_defRegsEnd
		MetatableFactory<MeshManager>::create(L, mshm);

		/////////////////////////////////////////////////////////////

		return 0;
	}
}}
