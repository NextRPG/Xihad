#include "BaseComponentSystem.h"

namespace xihad { namespace ngn
{
	
	InheritancePath BaseComponentSystem::hierarchy( const std::string& compName )
	{
		return mHierarchyTree.getInheritanceChain(compName);
	}

	BaseComponentSystem::BaseComponentSystem( const InheritanceTree& hierarchy ) :
		mHierarchyTree(hierarchy)
	{
	}

}}

