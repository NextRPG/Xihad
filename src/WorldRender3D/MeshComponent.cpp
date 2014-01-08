#include "MeshComponent.h"
#include "CppBase\xassert.h"
#include "irrlicht\IMeshSceneNode.h"
#include "irrlicht\ISceneManager.h"

using namespace irr::scene;
namespace xihad { namespace render3d
{
	
	MeshComponent::MeshComponent( const std::string& name, 
		ngn::GameObject& host, irr::scene::IMeshSceneNode* node ) :
		RenderComponent(name, host, node)
	{
	}

	void MeshComponent::setMesh( irr::scene::IMesh* mesh )
	{
		xassert(getNode());
		getNode()->setMesh(mesh);
	}

	irr::scene::IMesh* MeshComponent::getMesh()
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
			return true;
		}

		return false;
	}

	irr::scene::IMeshSceneNode* MeshComponent::getNode() const
	{
		return (irr::scene::IMeshSceneNode*) RenderComponent::getNode();
	}

}}

