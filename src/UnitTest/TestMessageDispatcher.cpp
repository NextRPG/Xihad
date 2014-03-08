#include "CppUnitTest.h"
#include <string>
#include <vector>
#include <iostream>
#include "GameWorld\Message\MessageDispatcher.h"
#include "GameWorld\Message\MessageTag.h"
#include "GameWorld\Message\MessageParam.h"
#include "GameWorld\Message\MessageListener.h"
#include "Lua\lua.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace xihad::ngn;
using namespace std;

namespace xihad { namespace UnitTest
{
	class Entity
	{
	public:
		typedef int Identifier;

		int id;
	};

	class EntityManager
	{
	public:
		Entity* findObject(Entity::Identifier id)
		{
			if (id < 0 || (size_t)id >= mEntities.size()) 
				return 0;

			return mEntities[id];
		}

		vector<Entity*> mEntities;
	};

	struct MessageParameter
	{
		MessageParameter(string tag, int arg) :
			tag(tag), arg(arg) {}

		const MessageTag& getTag() const { return tag; }

		MessageTag tag;
		int arg;
	};

	class Listener
	{
	public:
		typedef MessageParameter Parameter;
		Listener() : ref_count(1) {}
		virtual ~Listener() {}
		virtual void receive(Entity& entity, const Parameter& param) = 0;

		int ref_count;
	};

	void intrusive_ptr_add_ref(Listener* l)
	{
		++l->ref_count;
	}

	void intrusive_ptr_release(Listener* l)
	{
		--l->ref_count;
	}

	class CountingListener : public Listener
	{
	public:
		CountingListener(int id = 100) : receivedTimes(0) {}

		virtual void receive(Entity& entity, const Parameter& param)
		{
			++receivedTimes;
		}

		int receivedTimes;
	};

	typedef MessageDispatcher<Entity, EntityManager, Listener> UTDispatcher;
	TEST_CLASS(TestMessageDispatcher)
	{
	public:
		lua_State* L;
		EntityManager* mgr;
		UTDispatcher* mDispatcher;
		CountingListener listeners[5];
		Timeline t;

		TEST_METHOD_INITIALIZE(initState)
		{
			t.reset(0);

			for (int i = 0; i < 5; i++)
				listeners[i].receivedTimes = 0;

			L = luaL_newstate();
			mgr = new EntityManager;
			for (int i = 0; i < 10; ++i)
			{
				Entity* e = new Entity;
				e->id = i;
				mgr->mEntities.push_back(e);
			}

			mDispatcher = new UTDispatcher(*mgr);
			mDispatcher->start();

			mDispatcher->addListener(MessageTag("character"), &listeners[0]);
			mDispatcher->addListener(MessageTag("character.move"), &listeners[1]);
			mDispatcher->addListener(MessageTag("character.move.stop"), &listeners[2]);
			mDispatcher->addListener(MessageTag("character.action"), &listeners[3]);
			mDispatcher->addListener(MessageTag("round.start"), &listeners[4]);
		}

		void runCheck(int c1, int c2, int c3, int c4, int c5)
		{
			int arr[5];
			arr[0] = c1; arr[1] = c2; arr[2] = c3; arr[3] = c4; arr[4] = c5;

			this->mDispatcher->update(this->t);
			for (int i = 0; i < 5; ++i)
				Assert::AreEqual(listeners[i].receivedTimes, arr[i]);

			// reset
			for (int i = 0; i < 5; i++)
				listeners[i].receivedTimes = 0;
		}

		TEST_METHOD(TestHierarchicalReceive)
		{
			mDispatcher->dispatch(MessageParameter("character.move.stop", 1), 1);
			runCheck(1, 1, 1, 0, 0);

			mDispatcher->dispatch(MessageParameter("character.move", 1), 1);
			runCheck(1, 1, 0, 0, 0);

			mDispatcher->dispatch(MessageParameter("character", 1), 1);
			runCheck(1, 0, 0, 0, 0);

			mDispatcher->dispatch(MessageParameter("character.other", 1), 1);
			runCheck(1, 0, 0, 0, 0);

			mDispatcher->dispatch(MessageParameter("character.other.other", 1), 1);
			runCheck(1, 0, 0, 0, 0);

			mDispatcher->dispatch(MessageParameter("", 1), 1);
			runCheck(0, 0, 0, 0, 0);

#define NON_EXSIST_OBJECT_ID 100
			// Messages from non-exsist object will be discarded
			mDispatcher->dispatch(MessageParameter("character.move.stop", 1), NON_EXSIST_OBJECT_ID);
			runCheck(0, 0, 0, 0, 0);
		}
		
		TEST_METHOD(TestRemoveListener)
		{
			mDispatcher->addListener(MessageTag("character.dead"), &listeners[0]);
			
			mDispatcher->removeListener(MessageTag("character"), &listeners[0]);
			mDispatcher->dispatch(MessageParameter("character.dead", 1), 1);
			runCheck(0, 0, 0, 0, 0);

			mDispatcher->addListener(MessageTag("character.dead"), &listeners[1]);
			mDispatcher->addListener(MessageTag("a.b"), &listeners[1]);
			mDispatcher->addListener(MessageTag("a.d.d.d"), &listeners[1]);
			mDispatcher->clearListener(&listeners[1]);
			mDispatcher->dispatch(MessageParameter("a", 1), 1);
			runCheck(0, 0, 0, 0, 0);
			mDispatcher->dispatch(MessageParameter("character.dead", 1), 1);
			runCheck(0, 0, 0, 0, 0);

			// remove non-exist tag
			mDispatcher->removeListener(MessageTag("character.move.other"), &listeners[2]);
			mDispatcher->removeListener(MessageTag("character.move.stop.other"), &listeners[2]);
			mDispatcher->dispatch(MessageParameter("character.move.stop", 1), 1);
			runCheck(0, 0, 1, 0, 0);

			mDispatcher->removeListener(MessageTag("character.move.stop"), &listeners[2]);
			mDispatcher->dispatch(MessageParameter("character.move.stop", 1), 1);
			runCheck(0, 0, 0, 0, 0);

			// remove non-exist listener
			mDispatcher->removeListener(MessageTag("character.action"), &listeners[2]);
			mDispatcher->dispatch(MessageParameter("character.action", 1), 1);
			runCheck(0, 0, 0, 1, 0);

			mDispatcher->removeListener(MessageTag("test"), &listeners[0]);
			mDispatcher->addListener(MessageTag("test"), &listeners[0]);
			mDispatcher->dispatch(MessageParameter("test", 1), 1);
			runCheck(1, 0, 0, 0, 0 );
		}

		TEST_METHOD(TestDelayedDispatcher)
		{
			mDispatcher->dispatch(MessageParameter("character", 1), 1, 0.5);
			t.update(0.3f);
			runCheck(0, 0, 0, 0, 0);

			t.update(0.2000001f);
			runCheck(1, 0, 0, 0, 0);

			mDispatcher->dispatch(MessageParameter("character", 1), 1, 0.5);
			t.update(0.500001f);
			runCheck(1, 0, 0, 0, 0);
		}

		// This method will run after run every TestMethod in current class.
		TEST_METHOD_CLEANUP(cleanupState)
		{
			lua_close(L);

			for (Entity* e : mgr->mEntities)
				delete e;
			delete mgr;

			mDispatcher->destroy();
			for (int i = 0; i < 5; ++i)
				Assert::AreEqual(listeners[i].ref_count, 1);
		}
	};
}}
