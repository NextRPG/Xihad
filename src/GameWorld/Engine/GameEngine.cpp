#include "GameEngine.h"
#include <ctime>
#include <irrlicht/IrrlichtDevice.h>
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IFileSystem.h>
#include "GameWorld.h"
#include "Process.h"
#include "WorldRender3D/Geometry.h"
#include "GameScene.h"
#include "UserEvent.h"
#include "EngineStats.h"

using namespace irr;
namespace xihad { namespace ngn
{
	struct GameEngine::impl
	{
		float updateInterval;
		clock_t breakPointGuess;	// check for break point's big interval
		GameEngine::LoopStatus status;
		boost::shared_ptr<GameWorld> gameWorld;
		IEventReceiver* eventReceiver;
		irr_ptr<IrrlichtDevice> device;

		EngineStats stas;
		bool showFPS;
		bool neverSleep;

		impl(GameWorld* world) : gameWorld(world), stas(nullptr), 
			showFPS(false), neverSleep(false)
		{
		}
	};

	inline static clock_t toClock_t(float sec)
	{
		return (clock_t) (sec*CLOCKS_PER_SEC);
	}

	static IEventReceiver* createEventReceiver(GameWorld* world)
	{
		class EventReceiver : public IEventReceiver
		{
		public:
			explicit EventReceiver(GameWorld* world) : 
				mWorld(world)
			{
			}

			virtual bool OnEvent(const SEvent& event)
			{
				GameScene* scene;
				switch (event.EventType)
				{
				case EET_MOUSE_INPUT_EVENT:
					if (scene = mWorld->getScene())
						return scene->onForegroundEvent(event.MouseInput);
					break;
				case EET_KEY_INPUT_EVENT:
					if (scene = mWorld->getScene())
						return scene->onForegroundEvent(event.KeyInput);
					break;
				case EET_GUI_EVENT:
				case EET_JOYSTICK_INPUT_EVENT:
				case EET_LOG_TEXT_EVENT:
				case EET_USER_EVENT:
				default:
					// not support yet.
					break;
				}

				return false;
			}

		private:
			GameWorld* mWorld;
		};

		return new EventReceiver(world);
	}

	GameEngine::GameEngine(int frameCount)
	{
		float interval = 1.0f / frameCount;
		initWithWorld(new GameWorld(interval), interval);
	}

	GameEngine::GameEngine( GameWorld& world, int frameCount )
	{
		initWithWorld(&world, 1.0f/frameCount);
	}

	void GameEngine::initWithWorld( GameWorld* world, float frameInterval )
	{
		mImpl.reset(new impl(world));
		mImpl->updateInterval = frameInterval;
		mImpl->breakPointGuess = toClock_t(frameInterval * 100);
		mImpl->status = INITIALIZED;
	}

	GameEngine::~GameEngine()
	{
		mImpl->device->setEventReceiver(nullptr);
		delete mImpl->eventReceiver;
	}

	inline static float calcDelta(clock_t prev, clock_t maxInterval, float supposed) 
	{
		float ret = supposed;
		clock_t current = clock();
		clock_t delta =  current - prev;

		if (delta < maxInterval)
			ret = ((float) delta) / CLOCKS_PER_SEC;

		return ret;
	}

	inline static float waitForNextFrame(float updateInterval, float dtSeconds) 
	{
		float restTimeInFrame = updateInterval - dtSeconds;
		if (restTimeInFrame > 0.0f)
		{
			Process::sleep(restTimeInFrame);
			return updateInterval;
		}

		return dtSeconds;
	}

	bool GameEngine::launch()
	{
		if (isRunning() || mImpl->device.get() == nullptr)
			return false;

		LoopStatus& status = mImpl->status;
		// const clock_t& threshold = mImpl->breakPointGuess;
		const clock_t& threshold = mImpl->breakPointGuess;
		auto& world = mImpl->gameWorld;

		world->start();
		status = RUNNING;	// All updaters should be start before running.
		{
			float supposedInterval = mImpl->updateInterval;
			float stepTime = 0;
			float frameTime = supposedInterval;
			mImpl->device->setEventReceiver(mImpl->eventReceiver);

			while (status != STOPPED && mImpl->device->run())
			{
				// Everything will be done here
				clock_t timeBeforeStep = clock();
				world->step(frameTime);
				stepTime = calcDelta(timeBeforeStep, threshold, supposedInterval);

				// Engine Statistics or other stuff
				afterWorldStep(stepTime);

				if (mImpl->neverSleep)
					frameTime = stepTime;
				else 
					frameTime = waitForNextFrame(supposedInterval, stepTime);
			}
		}

		// sync state
		if (status == STOPPED)
			mImpl->device->closeDevice();
		else
			status = STOPPED;

		// call world to stop
		world->stop();

		return true;
	}

	bool GameEngine::isRunning() const
	{
		return mImpl->status == RUNNING;
	}

	bool GameEngine::stop()
	{
		if (mImpl->status == RUNNING)
		{
			mImpl->status = STOPPED;
		}

		return mImpl->status == STOPPED;
	}

	void GameEngine::setBreakPointThresholdTime( float thresholdSeconds )
	{
		thresholdSeconds = std::max(mImpl->updateInterval, thresholdSeconds);
		mImpl->breakPointGuess = toClock_t(thresholdSeconds);
	}

	float GameEngine::breakPointThresholdTime() const
	{
		return (float) mImpl->breakPointGuess / CLOCKS_PER_SEC;
	}

	boost::shared_ptr<GameWorld> GameEngine::getWorld()
	{
		return mImpl->gameWorld;
	}

	irr_ptr<irr::IrrlichtDevice> GameEngine::getDevice()
	{
		return mImpl->device;
	}

	bool GameEngine::initDevice( irr_ptr<irr::IrrlichtDevice> device )
	{
		if (mImpl->device.get() || !device.get())
			return false;

		mImpl->device = device;
		mImpl->eventReceiver = createEventReceiver(mImpl->gameWorld.get());
		mImpl->stas.resetDriver(device->getVideoDriver());

		return true;
	}

	void GameEngine::afterWorldStep( float stepTime )
	{
		EngineStats& stats = mImpl->stas;
		stats.recordFrame(stepTime);

		if (mImpl->showFPS)
		{
			std::wstring fps = L"XihadÒýÇæ[FPS: (T)";
			fps += std::to_wstring(stepTime);
			fps += L", (R)";
			fps += std::to_wstring(stats.getRecentFPS());
			fps += L"]";
			fps += L"[Primitive: (C)";
			fps += std::to_wstring(stats.getLastPrimitiveDrawn());
			fps += L", (A)";
			fps += std::to_wstring(stats.getAveragePrimitiveDrawnRate());
			fps += L"]";
			mImpl->device->setWindowCaption(fps.c_str());
		}
	}

	void GameEngine::setNeverSleep( bool neverSleep )
	{
		mImpl->neverSleep = neverSleep;
	}

	bool GameEngine::isNeverSleep() const
	{
		return mImpl->neverSleep;
	}

	void GameEngine::setShowFPS( bool showFPS )
	{
		mImpl->showFPS = showFPS;
	}

	bool GameEngine::isShowFPS() const
	{
		return mImpl->showFPS;
	}

}}
