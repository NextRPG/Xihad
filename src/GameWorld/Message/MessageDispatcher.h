#pragma once
#include "IMessageDispatcher.h"
#include <list>
#include <queue>
#include <functional>		// for std::greater
#include "SourceFilter.h"
#include "TagTree.h"
#include "MessageTag.h"
#include "Engine\Timeline.h"

namespace xihad { namespace ngn 
{
	template <typename Entity, typename EntityManager, typename Listener>
	class MessageDispatcher :
		public IMessageDispatcher<Entity, EntityManager, Listener>
 	{
	public:
		MessageDispatcher(EntityManager& manager) : mManager(&manager) {}

		void dispatch(ParamArg pParam, IdArg pSourceId, double timeout = 0.0) override
		{
			Event e(pParam, pSourceId);
			e.timeout = timeout;
			pendingEvents.push_back(e);
		}
		
		void addListener(const MessageTag& pEventTag, Listener* pListener) override
		{
			if (pListener != nullptr)
				listenerTree.insert(pEventTag, pListener);
		}

		void clearListener(Listener* pListener) override
		{
			if (pListener == nullptr) 
				return;

			pendingRemove.push_back(std::make_pair(MessageTag(), pListener));
		}

		void removeListener(const MessageTag& tag, Listener* pListener) override
		{
			if (tag.empty() || pListener == nullptr)
				return;

			pendingRemove.push_back(std::make_pair(tag, pListener));
		}

	protected:
		virtual ~MessageDispatcher()
		{
		}

		virtual void onStart() override
		{
		}

		virtual void onUpdate(const Timeline& pTimeline) override
		{
			// clear or remove listener until next update
			handlePendingRemovals();

			// insert pending messages into events queue
			handlePendingMessages(pTimeline);

			// dispatch timeout messages one by one
			auto& eventsQueue = awaitMessageQueue;
			while (!eventsQueue.empty())
			{
				auto& mostRencent = eventsQueue.top();
				if (mostRencent.timeout <= pTimeline.getElapsedSeconds())
				{
					dispatchMessageImpl(mostRencent.param, mostRencent.sourceId);
				 	eventsQueue.pop();
				}
				else
				{
					break;
				}
			}
		}

		virtual void onStop() override
		{
		}

	private:
		void dispatchMessageImpl(ParamArg pParam, IdArg pSourceId)
		{
			if (Entity* object = mManager->findObject(pSourceId))
			{
				TagTree<Listener*>::PathIterator pathItr = listenerTree.findPath(pParam.getTag());
				while (pathItr.hasNext())
				{
					for (Listener* listener : *pathItr)
						listener->receive(*object, pParam);

					++pathItr;
				}
			}
		}

		inline void handlePendingRemovals()
		{
			auto piter = pendingRemove.begin();
			while (piter != pendingRemove.end())
			{
				TagTree<Listener*>::SubtreeIterator tree_iter;
				if (piter->first.empty())
					tree_iter = listenerTree.begin();	// remove all 
				else
					tree_iter = listenerTree.findSubtree(piter->first);	// remove specified tag

				while (tree_iter.hasNext()) 
				{
					(*tree_iter).remove(piter->second);
					++tree_iter;
				}

				piter = pendingRemove.erase(piter);
			}
		}

		inline void handlePendingMessages(const Timeline& pTimeline)
		{
			double current = pTimeline.getElapsedSeconds();

			// 将新添加的消息事件添加进等待队列
			float indemnity = pTimeline.getLastTimeChange(); // 延时补偿

			int index = 0;
			auto it = pendingEvents.begin();
			while (it != pendingEvents.end())
			{
				it->timeout -= indemnity;
				it->timeout += current;	// 本地时间转换为全局时间
				it->arriveIndex = index;// 保证同样延迟的消息会先来先发
				awaitMessageQueue.push(*it);

				// 迭代器更新
				it = pendingEvents.erase(it);
				++index;
			}
		}

	private:
		struct Event
		{
			Event(ParamArg pParam, IdArg pSourceId)
				: param(pParam), sourceId(pSourceId), timeout(0), arriveIndex(0)
			{
			}

			bool operator>(const Event& pOther) const
			{
				if (timeout != pOther.timeout)
					return timeout > pOther.timeout;
				else 
					return arriveIndex > pOther.arriveIndex;
			}

			typename Listener::Parameter param;
			Identifier sourceId;
			double timeout;
			int arriveIndex;
		};

		typedef std::priority_queue<Event, std::vector<Event>, std::greater<Event> > MessageQueue;

		EntityManager* mManager;
		TagTree<Listener*> listenerTree;
		std::list<Event> pendingEvents;
		std::list<std::pair<MessageTag, Listener*> > pendingRemove;
		MessageQueue awaitMessageQueue;
	};
}}

