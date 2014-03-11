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
#include "TimeConversion.h"
#include "WindowEventSeizer.h"
#include "CppBase\StdMap.h"

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

	typedef multimap<int, xptr<FrameObserver>> FrameObservers;
	typedef unordered_map<
		xptr<FrameObserver>, 
		FrameObservers::iterator,
		hash_observer> Index;
	struct GameEngine::impl
	{
		float frameTime;
		GameEngine::LoopStatus status;

		boost::scoped_ptr<GameWorld> gameWorld;
		xptr<NativeWindow> window;

		FrameObservers frameObservers;
		Index observerIndex;
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
		if (auto value = StdMap::findValuePtr(mImpl->observerIndex, xptr<FrameObserver>(&observer)))
			mImpl->frameObservers.erase(*value);

		auto pos = mImpl->frameObservers.insert(std::make_pair(order, &observer));
		mImpl->observerIndex[&observer] = pos;
	}

	void GameEngine::removeFrameObserver( FrameObserver& observer )
	{
		xptr<FrameObserver> key(&observer);
		auto indexPos = mImpl->observerIndex.find(key);
		if (indexPos != mImpl->observerIndex.end())
		{
			auto pos = indexPos->second;
			mImpl->frameObservers.erase(pos);
			mImpl->observerIndex.erase(indexPos);
		}
	}

	float GameEngine::fireFrameBegin()
	{
		float now = clockToSeconds(clock());
		for (auto observerPair : mImpl->frameObservers)
			observerPair.second->onFrameBegin(this, now);

		return now;
	}

	void GameEngine::fireFrameEnd( float bgnTime )
	{
		float now = clockToSeconds(clock());
		float delta = now - bgnTime;

#ifdef _DEBUG
		int priority = 0x80000000;	// min integer
#endif
		for (auto& observerPair : mImpl->frameObservers)
		{
#ifdef _DEBUG
			assert(priority <= observerPair.first);
			priority = observerPair.first;
#endif
			observerPair.second->onFrameEnd(this, now, delta);
		}
	}

}}
