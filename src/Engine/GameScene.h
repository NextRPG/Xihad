#pragma once
#include "CompositeUpdateHandler.h"
#include <boost/scoped_ptr.hpp>
#include <list>
#include "GameObject.h"
#include "Message\IMessageDispatcher.h"
#include "Message\MessageListener.h"

struct lua_State;
namespace xihad { namespace ngn
{
	class ComponentSystem;
	class UserEventReceiverStack;
	/// 游戏场景对象
	/**
	 * 游戏场景负责管理其中的游戏对象，让游戏对象之间可以通讯。
	 * 
	 * TODO: 
	 *	- 一个游戏世界中，可以同时存在多个游戏场景
	 *  
	 * @author etnlGD
	 * @date 2013年12月13日 02:12:17
	 */
	class GameScene : public CompositeUpdateHandler
	{
	public:
		static const GameObject::Identifier sRootObjectID;

	public:
		typedef IMessageDispatcher<GameObject, GameScene, MessageListener> Dispatcher;
		typedef std::list<GameObject*> ObjectGroup;

	public:
		explicit GameScene();

		GameObject* getRootObject() const;

		Dispatcher* getDispatcher() const;

		/// 请求指定名字的组件系统
		/**
		 * 如果尚未创建该系统，则会尝试创建。成功则返回，否则返回空指针
		 */
		virtual ComponentSystem* requireSystem(const std::string& systemName);

		virtual bool hasSystem(const std::string& systemName) const;

		/// 查找具有指定 ID 的游戏对象
		/**
		 * @param id 指定查询 ID
		 * @return 拥有该 ID 的唯一的游戏对象，或者 NULL
		 */
		virtual GameObject* findObject(GameObject::IdArgType id);

		/// 查找具有指定标签的对象集合
		/**
		 * @return 场景中所有拥有该标签的对象集合，如果没有任何对象拥有该标签，则返回 NULL
		 */
		virtual const ObjectGroup* findObjectsByTag(const std::string& tag);

		/// 创建游戏对象
		/**
		 * @param id 指定 ID
		 * @param parent 指定父节点。默认为 NULL ，此时表示以根节点为父节点
		 * @return NULL 如果该 ID 已经被占用，或者指定父节点不属于本场景
		 */
		virtual GameObject* createObject(GameObject::IdArgType id, GameObject* parent = NULL);

		/// 创建拥有唯一标识符的游戏对象
		/** 
		 * 通常会有如下结果：
		 * <pre>
		 *	createUniqueObject("enemy"); -> object whose id is "enemy#0"
		 *	createUniqueObject("enemy"); -> object whose id is "enemy#1"
		 *	createUniqueObject("hero");	 -> object whose id is "hero#2"
		 * </pre>
		 * 所以，如果在之前 createObject("enemy-0") ，此方法仍然会因为名称冲突而失败。
		 * 
		 * @see createObject()
		 * @param header 生成的 ID 的前缀
		 * @param parent 指定父节点
		 * @param addHeaderToTag 默认为 true 的情况下，如果创建成功，那么该游戏对象会拥有 
		 *						 header 所表示的标签
		 */
		virtual GameObject* createUniqueObject(
			GameObject::TagArgType header, GameObject* parent = NULL, bool addHeaderToTag = true);

		virtual UserEventReceiverStack& getControllerStack();

	protected:	// Avoid delete
		virtual void onDestroy();

		virtual ~GameScene();

	private:
		friend class GameObject;
		virtual void onObjectDestroyed(GameObject* obj);

	private:
		struct impl;
		boost::scoped_ptr<impl> mImpl;
	};
}}

