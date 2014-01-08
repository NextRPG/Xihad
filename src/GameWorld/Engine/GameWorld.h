#pragma once
#include <boost\scoped_ptr.hpp>

namespace xihad { namespace ngn
{
	struct GameWorldImpl;
	class UpdateHandler;
	class GameScene;
	class GameWorld
	{
	public:
		explicit GameWorld(float defaultStepSize = 1.0f/60);

		virtual ~GameWorld();

		GameScene* setScene(GameScene* scene);
		
		GameScene* getScene() const;

		virtual void start();

		virtual void step(float dtSeconds);

		virtual void pause();

		virtual void resume();

		virtual void stop();

		virtual void appendUpdateHandler(UpdateHandler* updateHandler);

		virtual void removeUpdateHandler(UpdateHandler* updateHandler);

		float getStepSize() const;

		void setStepSize(float secs);

	private:
		struct impl;
		boost::scoped_ptr<impl> mImpl;
	};
}}