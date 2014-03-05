#include "GameWorld.h"
#include <algorithm>
#include "Timeline.h"
#include "CppBase/XiAssert.h"
#include "CompositeUpdateHandler.h"
#include "GameScene.h"

using namespace std;
namespace xihad { namespace ngn
{
	struct GameWorld::impl
	{
		Timeline timeline;
		CompositeUpdateHandler& updaters;
		GameScene* scene;

		GameWorld::impl() : updaters(*new CompositeUpdateHandler)
		{
		}
	};

	GameWorld::GameWorld( float defaultStepSize ) :
		mImpl(new impl)
	{
		mImpl->scene = nullptr;
		mImpl->timeline.setSingleStepSeconds(defaultStepSize);
	}

	GameWorld::~GameWorld()
	{
		mImpl->updaters.destroy();
	}

	GameScene* GameWorld::setScene( GameScene* scene )
	{
		if (!scene || mImpl->scene == scene) 
			return nullptr;

		GameScene* prev = mImpl->scene;

		auto iter = mImpl->updaters.findChildHandler(prev);
		if (iter != mImpl->updaters.childHandlerEnd())
			mImpl->updaters.eraseChildHandler(iter);

		mImpl->updaters.appendChildHandler(scene);
		mImpl->scene = scene;

		return prev;
	}

	void GameWorld::start()
	{
		if (mImpl->updaters.start())
		{
			// TODO: 
		}
	}

	void GameWorld::step( float dtSeconds )
	{
		if (mImpl->updaters.update(mImpl->timeline))
		{
			mImpl->timeline.update(dtSeconds);
		}
	}

	void GameWorld::stop()
	{
		if (mImpl->updaters.stop())
		{
			mImpl->timeline.reset(0.0f); 
			// TODO: any more operation?
		}
	}

	void GameWorld::pause()
	{
		mImpl->timeline.setPaused(true);
	}

	void GameWorld::resume()
	{
		mImpl->timeline.setPaused(false);
	}

	float GameWorld::getStepSize() const
	{
		return mImpl->timeline.getSingleStepSeconds();
	}

	void GameWorld::setStepSize( float secs )
	{
		mImpl->timeline.setSingleStepSeconds(secs);
	}

	void GameWorld::appendUpdateHandler( UpdateHandler* updateHandler )
	{
		mImpl->updaters.appendChildHandler(updateHandler);
	}

	void GameWorld::removeUpdateHandler( UpdateHandler* updateHandler )
	{
		mImpl->updaters.appendChildHandler(updateHandler);
	}

	GameScene* GameWorld::getScene() const
	{
		return mImpl->scene;
	}

}}

