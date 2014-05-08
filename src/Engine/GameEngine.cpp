#include "GameEngine.h"

// use in member field
#include <map>
#include <unordered_map>
#include "xptr.h"
#include "GameWorld.h"
#include "NativeWindow.h"
#include "FrameObserver.h"

// use in function
#include "WindowRenderer.h"
#include "WindowEventSeizer.h"
#include "CppBase\StdMap.h"
#include "BiAssociateMap.h"
#include "System.h"

using namespace std;
namespace xihad { namespace ngn
{
	struct hash_observer
	{
		size_t operator()(const xptr<FrameObserver>& key) const
		{
			return (size_t) (key.get());
		}
	};

	typedef multimap<int, xptr<FrameObserver> > OrderedObservers;
	typedef unordered_map<xptr<FrameObserver>, OrderedObservers::iterator, hash_observer> ObserverIndex;
	typedef BiAssociateMap<int, xptr<FrameObserver>, OrderedObservers, ObserverIndex> AssociateObserver;
	struct GameEngine::impl
	{
		float frameTime;
		GameEngine::LoopStatus status;

		boost::scoped_ptr<GameWorld> gameWorld;
		xptr<NativeWindow> window;
		AssociateObserver observers;
	};

	GameEngine::GameEngine(NativeWindow& wnd, GameWorld* world, float defaultFrameTime) :
		mImpl(new impl)
	{
		mImpl->gameWorld.reset(world ? world : new GameWorld(defaultFrameTime));
		mImpl->frameTime = defaultFrameTime;
		mImpl->status = INITIALIZED;
		mImpl->window = &wnd;
		
		// Auto seize window event into current scene
		xptr<WindowEventSeizer> wndObsv(new WindowEventSeizer(wnd), false);
		mImpl->gameWorld->addWorldObserver(*wndObsv);

		XIHAD_MLD_NEW_OBJECT;
	}

	GameEngine::~GameEngine()
	{
		XIHAD_MLD_DEL_OBJECT;
	}

	void GameEngine::setFrameTime( float dt )
	{
		mImpl->frameTime = dt;
	}

	bool GameEngine::launch()
	{
		// Avoid reentrant and restart
		if (isRunning() || mImpl->status == STOPPED) 
			return false;

		getWorld()->start();	// All updaters should be start before running.
		mImpl->status = RUNNING;

		while (isRunning())
		{
			double frameBgnTime = fireFrameBegin();

			getRenderer()->clearBuffer(true, true, SColor(255, 80, 120, 80));
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
		if (mImpl->status == INITIALIZED)
			mImpl->window->close();

		mImpl->status = STOPPED;
		return true;
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

	void GameEngine::addFrameObserver( FrameObserver& observer, int order )
	{
		mImpl->observers.add(order, &observer);
	}

	void GameEngine::removeFrameObserver( FrameObserver& observer )
	{
		mImpl->observers.remove(&observer);
	}

	double GameEngine::fireFrameBegin()
	{
		double now = System::getCurrentTime();
		for (auto observerPair : mImpl->observers.pairs())
			observerPair.second->onFrameBegin(this, (float) now);

		return now;
	}

	void GameEngine::fireFrameEnd( double bgnTime )
	{
		double now = System::getCurrentTime();
		float delta = (float) (now - bgnTime);

#ifdef _DEBUG
		int priority = 0x80000000;	// min integer
#endif
		for (auto& observerPair : mImpl->observers.pairs())
		{
#ifdef _DEBUG
			assert(priority <= observerPair.first);
			priority = observerPair.first;
#endif
			observerPair.second->onFrameEnd(this, (float) now, delta);
		}
	}

}}
