#include "CWrappedAnimatedMeshInitializer.h"
#include <irrlicht\IAnimatedMeshSceneNode.h>

using namespace irr::scene;
namespace xihad { namespace particle
{
	void CWrappedAnimatedMeshInitializer::setAnimatedNode( ISceneNode* node )
	{
		if (node->getType() == ESNT_ANIMATED_MESH)
			MeshIniter->setAnimatedMeshSceneNode(static_cast<IAnimatedMeshSceneNode*>(node));
		else
			std::cerr << "Specified scene node is not a AnimatedMeshNode" << std::endl;
	}

}}

