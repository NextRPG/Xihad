#pragma once
#include "WorldObserver.h"

namespace xihad { namespace ngn
{
	class NativeWindow;
	class WindowEventSeizer : public WorldObserver
	{
	public:
		WindowEventSeizer(NativeWindow& wnd) : wnd(wnd) {}

		virtual void onSceneChanged(GameWorld* world, GameScene* prev);
		
	private:
		NativeWindow& wnd;
	};
}}

