#include "CppUnitTest.h"
#include "GameWorld\Engine\CompositeUpdateHandler.h"
#include "GameWorld\Engine\Timeline.h"

// Assert::AreEqual(), ...
// Logger::WriteMessage()
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace xihad { namespace UnitTest 
{
	// TODO: 必须修改类名
	TEST_CLASS(TestUpdateHandler)
	{
	public:
		// This method will run before every TestMethod in current class.
		TEST_METHOD_INITIALIZE(initState)
		{
		}

		// TODO: 修改测试方法名，方便查看结果
		TEST_METHOD(TestDeleteUpdater)
		{
			ngn::CompositeUpdateHandler* comp = new ngn::CompositeUpdateHandler;
			class TestUpdater : public ngn::ManagedUpdateHandler
			{
			public:
				explicit TestUpdater()
				{
				}
			protected:
				virtual void onStart()
				{
				}

				virtual void onUpdate(const ngn::Timeline&)
				{
				}

				virtual void onStop()
				{
				}
			};

			ngn::UpdateHandler* u = new TestUpdater;
			comp->appendUpdateHandler(u);
			{
				comp->start();
				ngn::Timeline t;
				comp->update(t);
				Assert::AreEqual((int) u->status(), (int)ngn::UpdateHandler::UPDATED);
			}

			u->destroy();
			Assert::IsTrue(!comp->containsUpdateHandler(u));
		}

		// This method will run after run every TestMethod in current class.
		TEST_METHOD_CLEANUP(cleanupState)
		{
		}
	};
}}

