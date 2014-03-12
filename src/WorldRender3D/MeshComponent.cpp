#include "MeshComponent.h"
#include <CppBase\xassert.h>
#include <irrlicht\IMeshSceneNode.h>
#include <irrlicht\ISceneManager.h>
#include <Engine\Properties.h>
#include "MeshManager.h"

namespace xihad { namespace render3d
{
	using namespace ngn;
	using namespace core;

	MeshComponent::MeshComponent( const std::string& name, 
		ngn::GameObject& host, IMeshSceneNode* node ) :
		RenderComponent(name, host, node)
	{
	}

	void MeshComponent::setMesh( IMesh* mesh )
	{
		xassert(getNode());
		getNode()->setMesh(mesh);
	}

	IMesh* MeshComponent::getMesh()
	{
		xassert(getNode());
		return getNode()->getMesh();
	}

	bool MeshComponent::createTriangleSelector( SelectorInfo info )
	{
		IMeshSceneNode* node = getNode();
		ISceneManager* smgr = node->getSceneManager();
		ITriangleSelector* sel = nullptr;
		switch (info.type)
		{
		case SelectorType::Stupid:
			sel = smgr->createTriangleSelector(node->getMesh(), node);
			break;
		case SelectorType::Octree:
			sel = smgr->createOctreeTriangleSelector(
							node->getMesh(), node, info.param.minimalPolysPerNode);
			break;
		default:
			return RenderComponent::createTriangleSelector(info);
		}

		if (sel != nullptr)
		{
			node->setTriangleSelector(sel);
			sel->drop();
			return true;
		}

		return false;
	}

	IMeshSceneNode* MeshComponent::getNode() const
	{
		return (IMeshSceneNode*) RenderComponent::getNode();
	}

	MeshComponent* MeshComponent::create( 
		const std::string& compName, GameObject& obj, const ngn::Properties& param, 
		ISceneManager* smgr, MeshManager* msmgr )
	{
		IMesh* mesh = nullptr;
		if (const char* path = param.getString("mesh"))
			mesh = msmgr->getMesh(path);

		vector3df zero(0, 0, 0), one(1, 1, 1);
		IMeshSceneNode* meshNode = 
			smgr->addMeshSceneNode(mesh, 0, -1, zero, zero, one, true);

		return new MeshComponent(compName, obj, meshNode);
	}

}}

