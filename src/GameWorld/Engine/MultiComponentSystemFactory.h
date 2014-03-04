#pragma once
#include "ComponentSystemFactory.h"
#include "InheritenceTree.h"

namespace xihad { namespace ngn
{
	/// 多功用组件系统的工厂类
	/**
	 * 创建得到的组件系统可以创建多种类型的组件。该组件系统中只有两层继承树，所有非 
	 * @link getBaseTypeName() 基类型名 @endlink 的其余组件类型，均继承自基类型。
	 * 
	 * @author etnlGD
	 * @date 2013年12月13日 16:25:40
	 */
	class MultiComponentSystemFactory : public ComponentSystemFactory, protected InheritenceTree
	{
	public:
		/**
		 * @param mainSystem 指定组件系统的基类型名称
		 */
		MultiComponentSystemFactory(const std::string& mainSystem);

		virtual ComponentSystem* create(
			GameScene* scene, const std::string& typeName) final;

		/// 取得基类型名称
		const std::string& getBaseTypeName() const
		{
			return mBaseType;
		}

		/// 判断是否是基类型
		bool isBaseType(const std::string& typeName) const
		{
			return typeName == mBaseType;
		}

		/// 注册继承自 #getBaseTypeName() 的类型
		/**
		 * @see InheritenceTree::registerType()
		 */
		RegisterResult registerDerivedType(const std::string& typeName);

		InheritenceTree& inheritenceTree()
		{
			return *this;
		}

	protected:
		/// 为指定游戏场景创建组件系统
		/**
		 * 组件系统在单个游戏场景中是可重用的。有如下断言：
		 * <pre>
		 *	system1 = factory->create(scene, type1);
		 *	system2 = factory->create(theSameScene, type2);
		 *	assert(system1 == NULL || system2 == NULL || system1 == system2);
		 * </pre>
		 * 
		 * 只有第一次也是唯一一次为某个场景创建系统的时候，才会调用此方法。
		 */
		virtual ComponentSystem* createMainSystem(GameScene* scene) = 0;

	private:
		std::string mBaseType;
	};
}}

