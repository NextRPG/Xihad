#pragma once
#include "CppBase/ReferenceCounted.h"

namespace xihad { namespace ngn
{
	class GameScene;
	class GameWorld;
	class WorldObserver : public virtual ReferenceCounted
	{
	public:
		WorldObserver() { XIHAD_MLD_NEW_OBJECT; }
		virtual ~WorldObserver() { XIHAD_MLD_DEL_OBJECT; }

		virtual void onSceneChanged(GameWorld* world, GameScene* prev) = 0;
	};
}}

