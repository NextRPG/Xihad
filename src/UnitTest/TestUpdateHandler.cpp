#include "CppUnitTest.h"
#include "Engine\CompositeUpdateHandler.h"
#include "Engine\Timeline.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace xihad::ngn;

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework
{
	template <>
	static std::wstring ToString<UpdateHandler::Status>(const UpdateHandler::Status& q) 
	{
		return ToString((int) q);
	}
}}}

namespace xihad { namespace UnitTest 
{
	class StopWhenUpdateHandler : public UpdateHandler
	{
	public:
		static int deleteInvokeCount;
	protected:
		virtual ~StopWhenUpdateHandler() { ++ deleteInvokeCount; }
		virtual void onStart() {}
		virtual void onUpdate(const Timeline&) { this->stop(); }
		virtual void onStop() {}
	};

	class IdleHandler : public UpdateHandler
	{
	protected:
		virtual void onStart() {}
		virtual void onUpdate(const Timeline&) {}
		virtual void onStop() {}
	};

	
	class StatusVerifyHandler : public UpdateHandler
	{
	protected:
		virtual void onStart()					{ Assert::AreEqual(status(), STARTING); }
		virtual void onUpdate(const Timeline&)	{ Assert::AreEqual(status(), UPDATING); }
		virtual void onStop()					{ Assert::AreEqual(status(), STOPPING);}
		virtual void onDestroy()				{ Assert::AreEqual(status(), DESTROYING);}
	};

	int StopWhenUpdateHandler::deleteInvokeCount = 0;

	TEST_CLASS(TestUpdateHandler)
	{
		CompositeUpdateHandler* comp;
		ngn::Timeline t;
	public:
		TEST_METHOD_INITIALIZE(initState)
		{
			comp = new ngn::CompositeUpdateHandler;
		}

		TEST_METHOD(TestAutoDestroyAfterStopUpdater)
		{
			UpdateHandler* child = new StopWhenUpdateHandler;
			Assert::IsTrue(comp->appendChildHandler(child));
			Assert::IsTrue(comp->start());

			Assert::IsTrue(StopWhenUpdateHandler::deleteInvokeCount == 0);
			for (int i = 0; i < 5; ++i)
			{
				Assert::IsTrue(comp->update(t));
				t.update(0.016f);
			}
			Assert::IsTrue(StopWhenUpdateHandler::deleteInvokeCount == 1);
			Assert::IsTrue(!comp->containsChildHandler(child));
		}

		TEST_METHOD(TestCascadeStatus)
		{
			comp->appendChildHandler(new IdleHandler);
			comp->appendChildHandler(new IdleHandler);
			comp->appendChildHandler(new IdleHandler);

			Assert::AreEqual(comp->getChildHandlerCount(), 3u);
			for (auto it = comp->childHandlerBegin(); it != comp->childHandlerEnd(); ++it)
				Assert::AreEqual((*it)->status(), UpdateHandler::BORN);

			Assert::IsTrue(comp->start());
			for (int i = 0; i < 5; ++i)
			{
				Assert::AreEqual(comp->getChildHandlerCount(), 3u);
				for (auto it = comp->childHandlerBegin(); it != comp->childHandlerEnd(); ++it)
					Assert::AreEqual((*it)->status(), UpdateHandler::UPDATED);

				Assert::IsTrue(comp->update(t));
			}

			comp->stop();
			Assert::AreEqual(comp->getChildHandlerCount(), 3u);
			for (auto it = comp->childHandlerBegin(); it != comp->childHandlerEnd(); ++it)
				Assert::AreEqual((*it)->status(), UpdateHandler::DEAD);
		}

		TEST_METHOD(TestAppendChildAfterStart)
		{
			comp->start();

			UpdateHandler* idleChild = new IdleHandler;
			Assert::IsTrue(comp->appendChildHandler(idleChild));
			Assert::IsTrue(idleChild->status() == UpdateHandler::UPDATED);
		}

		TEST_METHOD(TestHandlerLifecycle)
		{
			UpdateHandler* h = comp;
			/// no start
			Assert::AreEqual(h->status(), UpdateHandler::BORN);
			Assert::IsFalse(h->update(t));
			Assert::AreEqual(h->status(), UpdateHandler::BORN);
			Assert::IsFalse(h->stop());
			Assert::AreEqual(h->status(), UpdateHandler::BORN);

			h = new StatusVerifyHandler;
			// started
			Assert::IsTrue(h->start());
			Assert::AreEqual(h->status(), UpdateHandler::UPDATED);
			Assert::IsTrue(h->update(t));	// verifing UpdateHandler::UPDATING;
			Assert::AreEqual(h->status(), UpdateHandler::UPDATED);
			Assert::IsFalse(h->start());
			Assert::AreEqual(h->status(), UpdateHandler::UPDATED);

			h->stop();	// verifying UpdateHandler::STOPPING;
			Assert::AreEqual(h->status(), UpdateHandler::DEAD);
			Assert::IsFalse(h->start());
			Assert::IsFalse(h->update(t));
			Assert::IsTrue(h->destroy());	// verifying UpdateHandler::DESTROYING;
			// h is deleted, so no more test
		}

		// This method will run after run every TestMethod in current class.
		TEST_METHOD_CLEANUP(cleanupState)
		{
			t.reset(0);
			Assert::IsTrue(comp->destroy());
		}
	};
}}

