#pragma once
#include <boost/call_traits.hpp>
#include "../UpdateHandler.h"

namespace xihad { namespace ngn
{
	class MessageTag;
	/// 消息分发者
	/**
	 * 接受游戏对象发送消息，然后根据消息的类别，在下次游戏更新的时候，再将消息分发给所有对此类消息
	 * 注册过的监听者。需要注意的是，如果注册的类型为 "a.b"， 那意味着将接受所有以 "a.b" 开头的 
	 * 消息，如 "a.b", "a.b.c"， 而"a.bb"不可以。
	 * 
	 * @author Alpha, etnlGD
	 * @date 2013年12月17日 10:10:12
	 */
	template <typename Entity, typename EntityManager, typename Listener>
	class IMessageDispatcher : public UpdateHandler
	{
	public:
		typedef typename Entity::Identifier Identifier;
		typedef typename boost::call_traits<Identifier>::param_type IdArg;

		typedef typename Listener::Parameter Parameter;
		typedef typename boost::call_traits<Parameter>::param_type ParamArg;

	public:
		/// 发布消息
		/**
		 * 记录消息，等待下一次更新的时候，添加至消息队列。添加到消息队列的同时，会根据延时计算出消息
		 * 真正发出去的时间。在消息超时后（即将发送消息之前），会确认 id 对应的游戏对象是否存在，如果
		 * 不存在则取消消息发送。
		 * 
		 * @see GameObject::getID() MessageListener
		 * @param pParam 发布的消息的参数，会传递给监听者
		 * @param pSourceId 消息发布者的 ID
		 * @param timeout 从加入消息队列开始到真正发布消息之间的延迟时间
		 */
		virtual void dispatch(ParamArg pParam, IdArg pSourceId, double timeout = 0.0) = 0;

		/// 添加监听者，监听某类标签的事件
		virtual void addListener(const MessageTag& pEventTag, Listener* pListener) = 0;

		/// 清除指定监听者的所有记录
		/**
		 * Remove pListener for all tags
		 * @param pListener Specify the listener who won't receive message anymore.
		 */
		virtual void clearListener(Listener* pListener) = 0;

		/// 让监听者不再监听此标签以及其子标签
		/**
		 * 如果某个对象监听了 "a.b" 和 "a" ，那么如果 removeListener(a, listener) 那么会
		 * 同时移除对 "a.b" 的监听
		 */
		virtual void removeListener(const MessageTag& tag, Listener* pListener) = 0;

		virtual void clear() = 0;

	protected:
		virtual ~IMessageDispatcher() {}
	};
}}
