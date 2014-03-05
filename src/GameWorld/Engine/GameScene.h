#pragma once
#include "CompositeUpdateHandler.h"
#include "boost\shared_ptr.hpp"
#include "UserEventReceiver.h"
#include "GameObject.h"
#include "Message\MessageParam.h"
#include "Message\IMessageDispatcher.h"
#include "Message\MessageListener.h"
#include <list>

struct lua_State;
namespace xihad { namespace ngn
{
	class ComponentSystem;
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
	class GameScene : public CompositeUpdateHandler, public UserEventReceiver
	{
	public:
		typedef IMessageDispatcher<GameObject, GameScene, MessageListener> Dispatcher;
		typedef std::list<boost::shared_ptr<UserEventReceiver>> ControllerStack;
		typedef std::list<GameObject*> ObjectGroup;

	public:
		explicit GameScene(lua_State* globalState = nullptr);

		lua_State* mainThread() const;

		/// 取得场景中的根节点对象
		GameObject* getRootObject() const;

		/// 取得消息的分发者
		Dispatcher* getDispatcher() const;

		virtual ComponentSystem* requireSystem(const std::string& systemName);

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

		virtual bool onForegroundEvent(const KeyEvent& event);

		virtual bool onForegroundEvent(const MouseEvent& event);

		virtual bool onBackgroundEvent( const KeyEvent& event );

		virtual bool onBackgroundEvent( const MouseEvent& event );

		/// 取得场景的用户事件控制器栈
		virtual ControllerStack& controllerStack() const;

	protected:
		virtual ~GameScene();

	private:
		friend class GameObject;
		virtual void onObjectDestroyed(GameObject* obj);

	public:
		/// 根节点的 ID 
		static const GameObject::Identifier sRootObjectID;

	private:
		struct impl;
		impl* mImpl;
	};
}}

