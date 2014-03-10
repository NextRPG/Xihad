#include "GameWorld.h"
#include <set>
#include "Timeline.h"
#include "GameScene.h"
#include "xptr.h"
#include "WorldObserver.h"
#include "TimelineObserver.h"

using namespace std;
namespace xihad { namespace ngn
{
	struct GameWorld::impl
	{
		Timeline timeline;
		GameScene* scene;

		float singleStepSeconds;
		float timeScale;
		bool paused;

		set<xptr<WorldObserver>> worldObservers;
		set<xptr<TimelineObserver>> timeObservers;
	};

	GameWorld::GameWorld( float defaultStepSize ) : mImpl(new impl)
	{
		mImpl->scene = nullptr;
		mImpl->singleStepSeconds = defaultStepSize;
		mImpl->timeScale = 1.0f;
		mImpl->paused = false;

		XIHAD_MLD_NEW_OBJECT;
	}

	GameWorld::~GameWorld()
	{
		if (mImpl->scene)
			mImpl->scene->destroy();

		XIHAD_MLD_DEL_OBJECT;
	}

	GameScene* GameWorld::setScene( GameScene* scene )
	{
		if (mImpl->scene != scene)
		{
			std::swap(scene, mImpl->scene);

			for (auto observer : mImpl->worldObservers)
				observer->onSceneChanged(this, scene);
		}

		return scene;
	}

	void GameWorld::start()
	{
		if (mImpl->scene && mImpl->scene->start())
		{
			// TODO: 
		}
	}

	void GameWorld::update( float dtSeconds )
	{
		if (mImpl->scene && mImpl->scene->update(mImpl->timeline))
		{
			float dtScaledSeconds = isPaused() ? 0 : dtSeconds*getTimeScale();
			mImpl->timeline.update(dtScaledSeconds);
		}
	}

	void GameWorld::stop()
	{
		if (mImpl->scene && mImpl->scene->stop())
		{
			mImpl->timeline.reset(0.0f); 
			// TODO: any more operation?
		}
	}

	GameScene* GameWorld::getScene() const
	{
		return mImpl->scene;
	}

	void GameWorld::setSingleStepSeconds( float secs )
	{
		mImpl->singleStepSeconds = secs;
	}

	float GameWorld::getSingleStepSeconds() const
	{
		return mImpl->singleStepSeconds;
	}

	void GameWorld::setPaused( bool puase )
	{
		if (mImpl->paused != puase)
		{
			mImpl->paused = puase;

			// Notify observers if pause state changed
			for (auto observer : mImpl->timeObservers)
				observer->onTimePaused(this);
		}
	}

	bool GameWorld::isPaused() const
	{
		return mImpl->paused;
	}

	void GameWorld::setTimeScale( float scale )
	{
		if (mImpl->timeScale != scale)
		{
			mImpl->timeScale = scale;

			// Notify observers if pause state changed
			for (auto observer : mImpl->timeObservers)
				observer->onTimeScaled(this);
		}
	}

	float GameWorld::getTimeScale() const
	{
		return mImpl->timeScale;
	}

	void GameWorld::singleStep()
	{
		if (isPaused())
			mImpl->timeline.update(getSingleStepSeconds());
	}

	void GameWorld::addWorldObserver( WorldObserver& observer )
	{
		mImpl->worldObservers.insert(&observer);
	}

	void GameWorld::removeWorldObserver( WorldObserver& observer )
	{
		mImpl->worldObservers.erase(&observer);
	}

	void GameWorld::addTimelineObserver( TimelineObserver& observer )
	{
		mImpl->timeObservers.insert(&observer);
	}

	void GameWorld::removeTimelineObserver( TimelineObserver& observer )
	{
		mImpl->timeObservers.erase(&observer);
	}


}}

