#pragma once
#include <string>
#include <vector>
#include "InheritenceChain.h"
#include "Properties.h"
#include "NullProperpties.h"

namespace xihad { namespace ngn
{
	class GameObject;
	class Component;

	/// 组件工厂接口
	/**
	 * @author etnlGD
	 * @date 2013年12月13日 16:46:46
	 */
	class ComponentFactory
	{
	public:
		virtual ~ComponentFactory() {}
		
		/// 获得指定类型的继承关系链
		/**
		 * 如果返回的关系链的 InheritenceChain::begin() == InheritenceChain::end() ，
		 * 则此工厂无法创建该类型名的组件。
		 * 
		 * @param typeName 指定组件类型名
		 */
		virtual InheritenceChain hierarchy(const std::string& typeName) = 0;

		/// 创建指定类型的组件
		/**
		 * 创建的同时，会将该组件附加到该游戏对象中
		 * 
		 * @param typeName 指定组件类型名
		 * @param hostObject 指定宿主对象，不能为空
		 * @param param 创建该组件时传递的参数对象
		 * @return 创建完成并附加到对象的组件，如果不能完成创建，返回 NULL
		 */
		virtual Component* create(const std::string& typeName, 
			GameObject& hostObject, const Properties& param = NullProperties()) = 0;
	};

}}

