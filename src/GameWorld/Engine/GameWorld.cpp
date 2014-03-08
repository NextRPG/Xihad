#include "GameWorld.h"
#include "Timeline.h"
#include "GameScene.h"

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
	};

	GameWorld::GameWorld( float defaultStepSize ) : mImpl(new impl)
	{
		mImpl->scene = nullptr;
		mImpl->singleStepSeconds = defaultStepSize;
		mImpl->timeScale = 1.0f;
		mImpl->paused = false;
	}

	GameWorld::~GameWorld()
	{
		if (mImpl->scene)
			mImpl->scene->destroy();
	}

	GameScene* GameWorld::setScene( GameScene* scene )
	{
		std::swap(scene, mImpl->scene);
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
		mImpl->paused = puase;
	}

	bool GameWorld::isPaused() const
	{
		return mImpl->paused;
	}

	void GameWorld::setTimeScale( float scale )
	{
		mImpl->timeScale = scale;
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


}}

