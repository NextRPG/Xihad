#pragma once
#include "CppBase/ReferenceCounted.h"

namespace xihad { namespace ngn
{
	class GameScene;
	class GameWorld;
	class WorldObserver : public virtual ReferenceCounted
	{
	public:
		virtual ~WorldObserver() {}

		virtual void onSceneChanged(GameWorld* world, GameScene* prev) = 0;
	};
}}

