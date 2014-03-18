#include "CDefaultEnv.h"
#include "CppBase\StringUtil.h"
#include <irrlicht\ISceneManager.h>
#include <irrlicht\IMeshSceneNode.h>
#include <irrlicht\IAnimatedMeshSceneNode.h>

using namespace irr::scene;
using namespace irr::core;
namespace xihad { namespace particle
{
	CDefaultEnv::CDefaultEnv( Node* source, Node* target ) :
		mSource(source), mTarget(target)
	{
	}

	IMesh* CDefaultEnv::getMesh( const char* meshDesc ) const
	{
		if (Node* n = getNode(meshDesc))
		{
			if (IMeshSceneNode* mn = dynamic_cast<IMeshSceneNode*>(n))
				return mn->getMesh();
			else if (IAnimatedMeshSceneNode* an = dynamic_cast<IAnimatedMeshSceneNode*>(n))
				return an->getMesh();
			else 
				return nullptr;
		}
		
		return mSource->getSceneManager()->getMesh(meshDesc);
	}

	ISceneNode* CDefaultEnv::getNode( const char* nodeDesc ) const
	{
		if (nodeDesc[0] == '@')
		{
			const char* const arr[] = { "source", "target", 0 };

			switch (StringUtil::select(nodeDesc+1, arr))
			{
			case 0:
				return mSource.get();
			case 1:
				return mTarget.get();
			}
		}

		return nullptr;
	}

	void CDefaultEnv::setPosition( ISceneNode* node, const vector3df& pos ) const
	{
		node->setPosition(pos);
	}

	irr::core::vector3df CDefaultEnv::getPosition( ISceneNode* node ) const
	{
		return node->getPosition();
	}

	irr::core::aabbox3df CDefaultEnv::getAABB( ISceneNode* node ) const
	{
		return node->getBoundingBox();
	}

}}

