#pragma once
#include "ComponentSystem.h"
#include "InheritenceTree.h"

namespace xihad { namespace ngn
{
	/// 采用继承树的组件系统
	class BaseComponentSystem : public ComponentSystem
	{
	public:
		/**
		 * @param hierarchy 层次关系必须和 #create() 方法实现对应。即，如果包含某个类型，那么
		 *					#create() 方法必须返回一个非空组件
		 */
		BaseComponentSystem(const InheritenceTree& hierarchy);

		virtual InheritenceChain hierarchy( const std::string& compName ) override;

	private:
		const InheritenceTree& mHierarchyTree;
	};
}}

