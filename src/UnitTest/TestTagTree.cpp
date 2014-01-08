#include "CppUnitTest.h"
#include <string>
#include <iostream>
#include "GameWorld/Message/TagTree.h"
#include "Message/ListenerWrapper.h"
#include "Message/MessageListener.h"

// Assert::AreEqual(), ...
// Logger::WriteMessage()
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace xihad { namespace UnitTest
{
	using namespace xihad::ngn;
	using std::string;

	class Listener : public MessageListener
	{
	public:
		Listener(string id): mId(id), receivedTimes(0) {}
		virtual ~Listener() {}

		virtual void receive( GameObject& pSource, const MessageParam& pEvent )
		{
			receivedTimes++;
		}

	public:
		string mId;
		int receivedTimes;
	};

	// TODO: 必须修改类名
	TEST_CLASS(TestTagTree)
	{
	public:
		// This method will run before every TestMethod in current class.
		TEST_METHOD_INITIALIZE(initState)
		{
		}

		// TODO: 修改测试方法名，方便查看结果
		TEST_METHOD(TestTagTreeBuild)
		{
			MessageTag tag1("character.move.left");
			MessageTag tag2("character.move.right");
			MessageTag tag3("skill.single.attack");
			MessageTag tag4("character.action.levelup");
			MessageTag tag5("music");
			MessageTag tag6("character.move");
			MessageTag tag7("character");
			TagTree<Listener> tree;

			tree.insert(tag1, Listener(tag1.toString()));
			tree.insert(tag2, Listener(tag2.toString()));
			tree.insert(tag3, Listener(tag2.toString()));
			tree.insert(tag4, Listener(tag2.toString()));
			tree.insert(tag5, Listener(tag2.toString()));
			tree.insert(tag6, Listener(tag2.toString()));
			tree.insert(tag7, Listener(tag2.toString()));
			tree.insert(tag4, Listener(tag2.toString()));

			auto i3 = tree.findPath(tag4);
			auto i4 = tree.findPath(tag3);
			auto i5 = tree.findPath(tag5);
			auto i6 = tree.find(tag4);

			std::string expected[] = { "character", "move", "left", "right", "action", "levelup",
									   "skill", "single", "attack", "music", };

			int index = 0;
			for (auto i = tree.begin(); i.hasNext(); ++i, ++index)
			{
				auto& list = (*i);
				Assert::AreEqual(, tag);
			}

		}

		// This method will run after run every TestMethod in current class.
		TEST_METHOD_CLEANUP(cleanupState)
		{
		}
	};
}}
