#include "CppUnitTest.h"
#include "Engine\GameScene.h"
#include "Engine\GameObject.h"
#include "Engine\Timeline.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace xihad::ngn;
namespace xihad { namespace UnitTest 
{
	TEST_CLASS(TestGameScene)
	{
	public:
		GameScene* scene;
		Timeline t;

		TEST_METHOD_INITIALIZE(initScene)
		{
			scene = new GameScene;
			scene->start();

			scene->createUniqueObject("enemy");
			scene->createUniqueObject("enemy");
			scene->createUniqueObject("hero");
			scene->createUniqueObject("hero");
			scene->createUniqueObject("hero");

			scene->findObject("enemy#0")->addTag("dead");
			scene->findObject("hero#2")->addTag("dead");
		}

		TEST_METHOD(TestTagIndexing)
		{
			auto enemies = scene->findObjectsByTag("enemy");
			Assert::AreEqual(2u, enemies->size());

			auto heroes = scene->findObjectsByTag("hero");
			Assert::AreEqual(3u, heroes->size());

			auto deadMan = scene->findObjectsByTag("dead");
			Assert::AreEqual(2u, deadMan->size());

			scene->findObject("enemy#1")->addTag("dead");
			Assert::AreEqual(3u, deadMan->size());

			scene->findObject("enemy#0")->removeTag("dead");
			Assert::AreEqual(2u, deadMan->size());
		}

		TEST_METHOD(TestFindByID)
		{
			GameObject* obj;

			Assert::IsNotNull(obj = scene->findObject("enemy#0"));
			Assert::AreEqual(obj->getID().c_str(), "enemy#0");

			Assert::IsNotNull(obj = scene->findObject("enemy#1"));
			Assert::AreEqual(obj->getID().c_str(), "enemy#1");

			Assert::IsNotNull(obj = scene->findObject("hero#2"));
			Assert::AreEqual(obj->getID().c_str(), "hero#2");

			Assert::IsNotNull(obj = scene->findObject("hero#3"));
			Assert::AreEqual(obj->getID().c_str(), "hero#3");

			Assert::IsNotNull(obj = scene->findObject("hero#4"));
			Assert::AreEqual(obj->getID().c_str(), "hero#4");
		}

		TEST_METHOD(TestDestroyObject)
		{
			scene->findObject("enemy#1")->destroy();
			Assert::IsNull(scene->findObject("enemy#1"));

			GameObject* e2 = scene->findObject("enemy#0");
			e2->stop();
			Assert::IsNotNull(scene->findObject("enemy#0"));
			scene->update(t);
			Assert::IsNull(scene->findObject("enemy#0"));

			auto enemies = scene->findObjectsByTag("enemy");
			Assert::IsNull(enemies);
		}

		TEST_METHOD_CLEANUP(cleanupState)
		{
			scene->destroy();
		}
	};
}}

