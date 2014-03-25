#pragma once
#include <string>
#include "UpdateHandler.h"
#include "BaseVisitable.h"

namespace xihad { namespace ngn
{
	class GameObject;
	class MessageTag;
	/// 游戏对象组件
	/**
	 * 组件的派生类的种类完全取决于游戏逻辑，引擎只提供基础组件，比如： RenderComponent, 
	 * LuaComponent. 组件用于添加到 GameObject 中，一个 GameObject 可以有任意多的组件。
	 * <p>
	 * 不采用组件技术的游戏引擎，一般会把组件的子类当作 GameObject 的父类，或者
	 * 将其实例化对象内置为 GameObject 的成员。这样的引擎是不通用和不易变的，在基于组件的游戏引
	 * 擎中，你可以在运行时，让 GameObject 继承任意组件类。然后通过 RTTI 技术，实现相同的功能。
	 * 
	 * @author etnlGD
	 * @date 2013年12月9日 20:28:16
	 */
	class Component : public UpdateHandler, public BaseVisitable
	{
	public:
		DEFINE_VISITABLE

	public:
		/**
		 * @see #getComponentName() #getHostObject()
		 * @param typeName 组件类别名
		 * @param host 组件附属游戏对象，不能为 NULL
		 */
		Component(const std::string& typeName, GameObject& host);

		/**
		 * @return 标识此组件的名字
		 */
		const std::string& getComponentName() const;

		/**
		 * @return 此组件所属的 GameObject
		 */
		GameObject* getHostObject() const;

	protected:
		virtual ~Component();

	private:
		std::string mTypeName;
		GameObject* mHost;
	};
}}

