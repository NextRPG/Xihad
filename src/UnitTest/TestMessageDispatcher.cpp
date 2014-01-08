#include "CppUnitTest.h"
#include <string>
#include <iostream>
#include "GameWorld\Message\MessageDispatcher.h"
#include "GameWorld\Message\MessageTag.h"
#include "GameWorld\Message\MessageParam.h"
#include "GameWorld\Message\MessageListener.h"
#include "Lua\lua.hpp"

// Assert::AreEqual(), ...
// Logger::WriteMessage()
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace xihad { namespace UnitTest
{
	using namespace xihad::ngn;
	using std::string;

	class ListenerExample : public MessageListener
	{
	public:
		ListenerExample(string id): mId(id), receivedTimes(0) {}
		virtual ~ListenerExample() {}

		virtual void receive( const MessageParam& pEvent, const string& pSourceId )
		{
			receivedTimes++;
			lastSourceId = pSourceId;
		}
	public:
		string mId;
		int receivedTimes;
		string lastSourceId;
	};

	// TODO: 必须修改类名
	TEST_CLASS(TestMessageDispatcher)
	{
	public:
		// This method will run before every TestMethod in current class.
		TEST_METHOD_INITIALIZE(initState)
		{
		}

		// TODO: 修改测试方法名，方便查看结果
		TEST_METHOD(TestNoconditionDispatch)
		{
			lua_State* L = luaL_newstate();
			MessageDispatcher* mDispatcher = new MessageDispatcher(L);
			MessageTag tag1 = MessageTag("character.action.stay");
			MessageTag tag2 = MessageTag("skill.single.attack");

			ListenerExample e1("character.action.stay");
			ListenerExample e2("skill");
			ListenerExample e3("skill.single");
			ListenerExample e4("skill.single.attack");
			ListenerExample e5("skill.single.attack.once");

			ListenerExample e6("skill.double");

			mDispatcher->addListener(tag1, &e1, nullptr);
			mDispatcher->addListener(tag2, &e2, nullptr);
			mDispatcher->addListener(MessageTag(e3.mId), &e3, nullptr);
			mDispatcher->addListener(MessageTag(e4.mId), &e4, nullptr);
			mDispatcher->addListener(MessageTag(e5.mId), &e5, nullptr);
			mDispatcher->addListener(MessageTag(e6.mId), &e6,nullptr);

			mDispatcher->dispatch(mDispatcher->newParam(tag1), "enemy-1", DispatchCondition());
			mDispatcher->dispatch(mDispatcher->newParam(tag2), "enemy-2", DispatchCondition());

			Assert::AreEqual(e1.receivedTimes, 1);
			Assert::AreEqual(e2.receivedTimes, 1);
			Assert::AreEqual(e3.receivedTimes, 1);
			Assert::AreEqual(e4.receivedTimes, 1);

			Assert::AreEqual(e5.receivedTimes, 0);
			Assert::AreEqual(e6.receivedTimes, 0);

			Assert::AreEqual(e1.lastSourceId, string("enemy-1"));
			Assert::AreEqual(e2.lastSourceId, string("enemy-2"));
			Assert::AreEqual(e3.lastSourceId, string("enemy-2"));
			Assert::AreEqual(e4.lastSourceId, string("enemy-2"));

			mDispatcher->destroy();
		}

		TEST_METHOD(TestDelayedDispatcher)
		{

		}

		// This method will run after run every TestMethod in current class.
		TEST_METHOD_CLEANUP(cleanupState)
		{
		}
	};
}}
