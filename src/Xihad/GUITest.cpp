// #include <string>
// #include "XihadInitializer.h"
// #include "Engine/GameScene.h"
// #include "Engine/GameWorld.h"
// #include "Engine/GameEngine.h"
// #include "CppBase/XiAssert.h"
// #include "SceneCreator.h"
// #include "CreateDevice.h"
// #include "irrlicht/IrrlichtDevice.h"
// #include "Engine/IrrlichtWindow.h"
// #include "Engine/FPSCounter.h"
// #include "Engine/WindowTitleUpdater.h"
// #include "Engine/WindowEventTransmitter.h"
// #include "Engine/FrameRateAdjuster.h"
// #include "Engine/UserEventReceiverStack.h"
// #include "Engine/UpdateHandler.h"
// #include "Engine/Timeline.h"
// #include "CEGUI/System.h"
// #include "CEGUI/GUIContext.h"
// #include "CEGUI/Window.h"
// #include "CEGUIBasedDialogue/Conversation.h"
// #include "CEGUI/WindowManager.h"
// #include "CEGUIBasedDialogue/SpeakerSupport.h"
// #include <set>
// 
// using namespace irr::scene;
// using namespace xihad;
// using namespace ngn;
// using namespace dialogue;
// 
// static SpeakerSupport* speaker1;
// static SpeakerSupport* speaker2;
// Conversation* createConversation()
// {
// 	
// 	Conversation* conversation = new Conversation();
// 	auto& coder = CEGUI::System::getStringTranscoder();
// 	speaker1 = conversation->newSpeaker(coder.stringFromStdWString(L"路飞"));
// 	speaker1->setIcon("Character/aaaa_happy");
// 	speaker1->setIconRelativeX(0.1f);
// 	speaker1->setDialoguePosition(10, -10);
// 
// 	speaker2 = conversation->newSpeaker(coder.stringFromStdWString(L"路飞"));
// 	speaker2->setIcon("Character/bbbb_sad");
// 	speaker2->setIconRelativeX(-0.1f);
// 	speaker2->setDialoguePosition(-10, -10);
// 
// 	return conversation;
// }
// 
// 	class DialogueController: public UserEventReceiver
// 	{
// 	public:
// 		DialogueController(SpeakerSupport* s1, SpeakerSupport* s2, 
// 			UpdateHandler* u, GameScene* s) :
// 			s1(s1), s2(s2), updater(u), scene(s)
// 		{
// 			auto& coder = CEGUI::System::getStringTranscoder();
// 			words.push_back(coder.stringFromStdWString(
// 				L"想多角色我的开始， 叫上我，饿多看得我。多角色。"));
// 			words.push_back(coder.stringFromStdWString(
// 				L"搜救队网段看我呢，的，的， 夹克衫。打开多了的肯德基喇肯定。 的。"));
// 			words.push_back(coder.stringFromStdWString(
// 				L"快手快脚的卡卡卡卡卡卡卡卡卡卡卡卡卡卡卡卡卡卡卡卡。说的kd.s看懂了。的老师。 是"));
// 		}
// 
// 		virtual ~DialogueController() {}
// 
// 		virtual int onKeyEvent(const KeyEvent& event, int argFromPreviousReceiver) 
// 		{
// 			int index = std::rand() % words.size();
// 			if (event.Key == KeyCode::KEY_KEY_A && event.PressedDown)
// 			{
// 				s2->deactive();
// 				s1->open();
// 				s1->active();
// 				s1->speak(words[index]);
// 			}
// 			else if (event.Key == KeyCode::KEY_KEY_B && event.PressedDown)
// 			{
// 				s1->deactive();
// 				s2->open();
// 				s2->active();
// 				s2->speak(words[index]);
// 			}
// 			else if (event.Key == KeyCode::KEY_SPACE && event.PressedDown)
// 			{
// 				s1->setSpeed(6);
// 				s2->setSpeed(6);
// 			}
// 			else if (event.Key == KeyCode::KEY_SPACE && !event.PressedDown)
// 			{
// 				s1->setSpeed(1);
// 				s2->setSpeed(1);
// 			}
// 			else if (event.Key == KeyCode::KEY_KEY_Z && !event.PressedDown)
// 			{
// 				s1->close();
// 				s2->close();
// 			}
// 			else if (event.Key == KeyCode::KEY_KEY_D && !event.PressedDown)
// 			{
// 				scene->destroyChildHandler(updater);
// 			}
// 
// 			//s1->speakAll();
// 
// 			return 0;
// 		}
// 
// 		virtual int onMouseEvent(const MouseEvent& event, int argFromPreviousReceiver)
// 		{
// 			return 1;
// 		}
// 	private:
// 		std::vector<String> words;
// 		SpeakerSupport* s1;
// 		SpeakerSupport* s2;
// 		UpdateHandler* updater;
// 		GameScene* scene;
// 	};
// 
// 
// int cegui_test(int argc, const char** argv)
// {
// 	irr::IrrlichtDevice* device = createDefaultDevice();
// 	IrrlichtWindow* wnd = new IrrlichtWindow(*device);
// 	GameEngine* engine = new GameEngine(*wnd);
// 	initSystem(engine);
// 
// 	if (GameScene* scene = createScene("Assets/test/boot_cegui.lua"))
// 	{
// 		Conversation* conversation = createConversation();
// 		scene->appendChildHandler(conversation);
// 		scene->getControllerStack().pushReceiver(new DialogueController(speaker1, speaker2, conversation, scene));
// 		// scene->destroyChildHandler(conversation);
// 		///////////////////////////////////////
// 
// 		FrameRateAdjuster* adj = new FrameRateAdjuster(1.f/60);
// 		engine->addFrameObserver(*adj);
// 		adj->drop();
// 
// 		WindowTitleUpdater* titleUpdater = new WindowTitleUpdater;
// 		engine->addFrameObserver(*titleUpdater);
// 
// 		FPSCounter* counter = new FPSCounter;
// 		titleUpdater->setFPSCounter(counter);
// 		engine->addFrameObserver(*counter);
// 		counter->drop();
// 
// 		if (argc>2 && strcmp(argv[2], "-showfps") == 0)
// 			titleUpdater->setShowFPS(true);
// 
// 		titleUpdater->drop();
// 
// 		WindowEventTransmitter* eventTransmitter = new WindowEventTransmitter;
// 		engine->addFrameObserver(*eventTransmitter);
// 		eventTransmitter->drop();
// 
// 		engine->getWorld()->setScene(scene);
// 		engine->launch();
// 		delete engine;
// 	}
// 
// 	wnd->drop();
// 	device->drop();
// 	destroySystems();
// 
// 	system("pause");
// 	return 0;
// 
// }
// 
