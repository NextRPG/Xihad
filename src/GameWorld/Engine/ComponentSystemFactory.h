#pragma once
#include <string>

namespace xihad { namespace ngn
{
	class ComponentSystem;
	class GameScene;
	/// 组件系统的工厂类
	/**
	 * 一个工厂可以创建多种系统，每种系统有一个对应的标识符。该标识符表示，这个系统可以创建
	 * 这种类型的组件。通常用法如下：
	 * <pre>
	 *		system = systemFactory->create(scene, "mesh");
	 *		component = system->create("mesh", object, param);
	 * </pre>
	 * 
	 * @author etnlGD
	 * @date 2013年12月13日 16:21:38
	 */
	class ComponentSystemFactory
	{
	public:
		virtual ~ComponentSystemFactory() {}

		/// 创建或重用组件系统
		/**
		 * @see GameScene::requireSystem()
		 * @param scene 指定请求创建组件系统的游戏场景
		 * @param typeName 指定组件类型名
		 * @return 返回一个可以创建 typeName 所指定的组件类型的组件系统
		 */
		virtual ComponentSystem* create(
			GameScene* scene, const std::string& typeName) = 0;
	};
}}

