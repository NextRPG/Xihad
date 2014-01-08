#include "CppUnitTest.h"
#include "GameWorld\Engine\GameScene.h"
#include "GameWorld\Engine\GameObject.h"

// Assert::AreEqual(), ...
// Logger::WriteMessage()
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace xihad::ngn;
namespace xihad { namespace UnitTest 
{
	// TODO: 必须修改类名
	TEST_CLASS(TestGameScene)
	{
	public:
		GameScene* scene;

		// This method will run before every TestMethod in current class.
		TEST_METHOD_INITIALIZE(initScene)
		{
			scene = new GameScene;
			scene->createUniqueObject("enemy");
			scene->createUniqueObject("enemy");
			scene->createUniqueObject("hero");
			scene->createUniqueObject("hero");
			scene->createUniqueObject("hero");

			scene->findObject("enemy-0")->addTag("dead");
			scene->findObject("hero-2")->addTag("dead");
		}

		// TODO: 修改测试方法名，方便查看结果
		TEST_METHOD(TestTagIndexing)
		{
			auto enemies = scene->findObjectsByTag("enemy");
			Assert::AreEqual(2, (int) enemies->size());

			auto heroes = scene->findObjectsByTag("hero");
			Assert::AreEqual(3, (int) heroes->size());

			auto deadMan = scene->findObjectsByTag("dead");
			Assert::AreEqual(2, (int) deadMan->size());

			scene->findObject("enemy-1")->addTag("dead");
			Assert::AreEqual(3, (int) deadMan->size());

			scene->findObject("enemy-0")->removeTag("dead");
			Assert::AreEqual(2, (int) deadMan->size());
		}

		TEST_METHOD(TestFindByID)
		{
			GameObject* obj;

			Assert::IsNotNull(obj = scene->findObject("enemy-0"));
			Assert::AreEqual(obj->getID().c_str(), "enemy-0");

			Assert::IsNotNull(obj = scene->findObject("enemy-1"));
			Assert::AreEqual(obj->getID().c_str(), "enemy-1");

			Assert::IsNotNull(obj = scene->findObject("hero-2"));
			Assert::AreEqual(obj->getID().c_str(), "hero-2");

			Assert::IsNotNull(obj = scene->findObject("hero-3"));
			Assert::AreEqual(obj->getID().c_str(), "hero-3");

			Assert::IsNotNull(obj = scene->findObject("hero-4"));
			Assert::AreEqual(obj->getID().c_str(), "hero-4");
		}

		TEST_METHOD(TestDestroyObject)
		{
			scene->destroyObject("enemy-1");
			Assert::IsNull(scene->findObject("enemy-1"));

			auto enemies = scene->findObjectsByTag("enemy");
			Assert::AreEqual(1, (int) enemies->size());

			GameObject* e2 = scene->findObject("enemy-0");
			e2->destroy();
			Assert::IsNull(scene->findObject("enemy-0"));

			enemies = scene->findObjectsByTag("enemy");
			Assert::IsNull(enemies);
		}

		// This method will run after run every TestMethod in current class.
		TEST_METHOD_CLEANUP(cleanupState)
		{
			scene->destroy();
		}
	};
}}

