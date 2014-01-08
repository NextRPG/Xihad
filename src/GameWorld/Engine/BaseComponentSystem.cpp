#include "BaseComponentSystem.h"

namespace xihad { namespace ngn
{
	
	InheritenceChain BaseComponentSystem::hierarchy( const std::string& compName )
	{
		return mHierarchyTree.getInheritenceChain(compName);
	}

	BaseComponentSystem::BaseComponentSystem( const InheritenceTree& hierarchy ) :
		mHierarchyTree(hierarchy)
	{
	}

}}

