#include "GameEngine.h"

// use in member field
#include <set>
#include "irr_ptr.h"
#include "GameWorld.h"
#include "NativeWindow.h"
#include "FrameObserver.h"

// use in function
#include "WindowRenderer.h"
#include "TimeConversion.h"
#include "WindowEventSeizer.h"

using namespace irr;
using namespace std;
namespace xihad { namespace ngn
{
	struct GameEngine::impl
	{
		float frameTime;
		GameEngine::LoopStatus status;

		boost::scoped_ptr<GameWorld> gameWorld;
		irr_ptr<NativeWindow> window;

		set<irr_ptr<FrameObserver>> frameObservers;
	};

	GameEngine::GameEngine(NativeWindow& wnd, GameWorld* world, float defaultFrameTime) :
		mImpl(new impl)
	{
		mImpl->gameWorld.reset(world ? world : new GameWorld(defaultFrameTime));
		mImpl->frameTime = defaultFrameTime;
		mImpl->status = INITIALIZED;
		mImpl->window = &wnd;
		
		// Auto seize window event into current scene
		irr_ptr<WindowEventSeizer> wndObsv(new WindowEventSeizer(wnd), false);
		mImpl->gameWorld->addWorldObserver(*wndObsv);
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

		getWorld()->start();	// All updaters should be start before running.
		mImpl->status = RUNNING;

		while (isRunning())
		{
			float frameBgnTime = fireFrameBegin();

			getRenderer()->clearBuffer(true, true, SColor(255, 100, 100, 140));
			getWorld()->update(mImpl->frameTime);
			getRenderer()->swapBuffer();

			fireFrameEnd(frameBgnTime);
		}

		mImpl->window->close();
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

	GameWorld* GameEngine::getWorld()
	{
		return mImpl->gameWorld.get();
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
