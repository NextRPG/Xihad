#include "CppUnitTest.h"
#include "GameWorld\Message\DispatchCondition.h"
#include "GameWorld\Message\MessageTag.h"

// Assert::AreEqual(), ...
// Logger::WriteMessage()
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace xihad { namespace UnitTest
{
	// TODO: 必须修改类名
	TEST_CLASS(TestDispatchCondition)
	{
	public:
		// This method will run before every TestMethod in current class.
		TEST_METHOD_INITIALIZE(initState)
		{
		}

		// TODO: 修改测试方法名，方便查看结果
		TEST_METHOD(TestConditionInialState)
		{
			using namespace xihad::ngn;
			DispatchCondition c1;

			Assert::IsTrue(c1.timeOut(0));

			DispatchCondition c2(1230);
			c2.setRegisterTime(100);
			Assert::IsFalse(c2.timeOut(1230));
			Assert::IsTrue(c2.timeOut(1330));
			Assert::IsTrue(c2.timeOut(1340));
		}

		// This method will run after run every TestMethod in current class.
		TEST_METHOD_CLEANUP(cleanupState)
		{
		}
	};
}}


