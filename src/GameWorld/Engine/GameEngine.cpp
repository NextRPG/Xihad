#include "GameEngine.h"

// use in member field
#include <set>
#include "GameWorld.h"
#include "FrameObserver.h"
#include "NativeWindow.h"

// use in function
#include "WindowRenderer.h"
#include "TimeConversion.h"


// TODO
#include "GameScene.h"
#include "UserEventReceiverStack.h"

using namespace irr;
namespace xihad { namespace ngn
{
	struct GameEngine::impl
	{
		float frameTime;
		GameEngine::LoopStatus status;
		boost::shared_ptr<GameWorld> gameWorld;
		irr_ptr<NativeWindow> window;
		std::set<irr_ptr<FrameObserver>> frameObservers;

		impl(GameWorld* world) : gameWorld(world) {}
	};

	GameEngine::GameEngine(NativeWindow& wnd, float ft)
	{
		init(wnd, new GameWorld(ft), ft);
	}

	GameEngine::GameEngine(NativeWindow& wnd, GameWorld& world, float ft)
	{
		init(wnd, &world, ft);
	}

	void GameEngine::init(NativeWindow& wnd, GameWorld* world, float frameInterval)
	{
		mImpl.reset(new impl(world));
		mImpl->frameTime = frameInterval;
		mImpl->status = INITIALIZED;
		mImpl->window = &wnd;
	}

	GameEngine::~GameEngine()
	{
	}

	void GameEngine::setFrameTime( float dt )
	{
		mImpl->frameTime = dt;
	}

	bool GameEngine::launch()
	{
		// Avoid reentrant
		if (isRunning()) return false;

		getWorld()->start();
		mImpl->status = RUNNING;	// All updaters should be start before running.

		while (isRunning())
		{
			// TODO
			getWindow()->setEventReceiver(&getWorld()->getScene()->getControllerStack());

			float frameBgnTime = fireFrameBegin();

			getRenderer()->clearBuffer();
			getWorld()->update(mImpl->frameTime);
			getRenderer()->swapBuffer();

			fireFrameEnd(frameBgnTime);
		}

		// sync state
		if (mImpl->status == STOPPED)
			mImpl->window->close();
		else
			mImpl->status = STOPPED;

		// call world to stop
		getWorld()->stop();

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

	boost::shared_ptr<GameWorld> GameEngine::getWorld()
	{
		return mImpl->gameWorld;
	}

	NativeWindow* GameEngine::getWindow()
	{
		return mImpl->window.get();
	}

	WindowRenderer* GameEngine::getRenderer()
	{
		return mImpl->window->getRenderer();
	}

	void GameEngine::addFrameObserver( FrameObserver& observer )
	{
		mImpl->frameObservers.insert(&observer);
	}

	void GameEngine::removeFrameObserver( FrameObserver& observer )
	{
		mImpl->frameObservers.erase(&observer);
	}

	float GameEngine::fireFrameBegin()
	{
		float now = clockToSeconds(clock());
		for (irr_ptr<FrameObserver> observer : mImpl->frameObservers)
			observer->onFrameBegin(this, now);

		return now;
	}

	void GameEngine::fireFrameEnd( float bgnTime )
	{
		float now = clockToSeconds(clock());
		float delta = now - bgnTime;

		for (irr_ptr<FrameObserver> observer : mImpl->frameObservers)
			observer->onFrameEnd(this, now, delta);
	}

}}
