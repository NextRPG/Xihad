#pragma once
#include "UpdateHandler.h"
#include "ComponentFactory.h"
#include "MemoryLeakDetector.h"

namespace xihad { namespace ngn
{
	/// 组件系统
	/**
	 * 组件系统是可更新对象，并且负责创建组件
	 * 
	 * @author etnlG
	 * @date 2013年12月13日 16:46:06
	 */
	class ComponentSystem : public UpdateHandler, public ComponentFactory
	{
	public:
		ComponentSystem() { XIHAD_MLD_NEW_OBJECT; }
		virtual ~ComponentSystem() { XIHAD_MLD_DEL_OBJECT; }
	};
}}

