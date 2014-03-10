#pragma once
#include "IMessageDispatcher.h"
#include <list>
#include <queue>
#include <boost\intrusive_ptr.hpp>
#include <functional>		// for std::greater
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
		MessageDispatcher(EntityManager& manager) : mManager(&manager) 
		{
			XIHAD_MLD_NEW_OBJECT;
		}

		void dispatch(ParamArg pParam, IdArg pSourceId, double timeout = 0.0) override
		{
			Event e(pParam, pSourceId);
			e.timeout = timeout;
			pendingEvents.push_back(e);
		}
		
		void addListener(const MessageTag& pEventTag, Listener* pListener) override
		{
			if (!pEventTag.empty() && pListener)
				pendingOps.push_back(PendingOps(ADD, pEventTag, pListener));
		}

		void clearListener(Listener* pListener) override
		{
			if (pListener) 
				pendingOps.push_back(PendingOps(CLEAR, MessageTag(), pListener));
		}

		void removeListener(const MessageTag& tag, Listener* pListener) override
		{
			if (!tag.empty() && pListener)
				pendingOps.push_back(PendingOps(REMOVE, tag, pListener));
		}

	protected:
		virtual ~MessageDispatcher()
		{
			XIHAD_MLD_DEL_OBJECT;
		}

		virtual void onStart() override
		{
		}

		virtual void onUpdate(const Timeline& pTimeline) override
		{
			// clear or remove listener until next update
			handlePendingOps();

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
			pendingEvents.clear();
			pendingOps.clear();
			listenerTree.clear();
		}

	private:
		void dispatchMessageImpl(ParamArg pParam, IdArg pSourceId)
		{
			if (Entity* object = mManager->findObject(pSourceId))
			{
				TagTreeT::PathIterator pathItr = listenerTree.findPath(pParam.getTag());
				while (pathItr.hasNext())
				{
					for (boost::intrusive_ptr<Listener> listener : *pathItr)
						listener->receive(*object, pParam);

					++pathItr;
				}
			}
		}

		inline void handlePendingOps()
		{
			auto piter = pendingOps.begin();
			while (piter != pendingOps.end())
			{
				if (piter->opcode == ADD)
				{
					listenerTree.insert(piter->tag, piter->listener);
				}
				else
				{
					TagTreeT::SubtreeIterator tree_iter;
					switch (piter->opcode)
					{
					case CLEAR:
						tree_iter = listenerTree.begin();
						break;
					case REMOVE:
						tree_iter = listenerTree.findSubtree(piter->tag);
						break;
					}

					while (tree_iter.hasNext()) 
					{
						(*tree_iter).remove(piter->listener);
						++tree_iter;
					}
				}

				piter = pendingOps.erase(piter);
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

		enum PendingOpCode { ADD, REMOVE, CLEAR };
		struct PendingOps
		{
			PendingOps(PendingOpCode opcode, const MessageTag& tag, Listener* li) :
				opcode(opcode), tag(tag), listener(li) {}

			PendingOpCode opcode;
			MessageTag tag;
			boost::intrusive_ptr<Listener> listener;
		};

		typedef std::priority_queue<Event, std::vector<Event>, std::greater<Event> > MessageQueue;
		typedef TagTree<boost::intrusive_ptr<Listener> > TagTreeT;

		EntityManager* mManager;
		TagTreeT listenerTree;
		std::list<Event> pendingEvents;
		std::list<PendingOps> pendingOps;
		MessageQueue awaitMessageQueue;
	};
}}

