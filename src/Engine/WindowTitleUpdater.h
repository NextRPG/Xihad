#pragma once
#include "FrameObserver.h"
#include <string>
#include "xptr.h"

namespace xihad { namespace ngn
{
	class FPSCounter;
	class WindowTitleUpdater : public FrameObserver
	{
	public:
		WindowTitleUpdater(const std::wstring& appName = L"Xihad", FPSCounter* = 0);

		virtual ~WindowTitleUpdater();

		virtual void onFrameBegin( GameEngine* source, float nowSecs );

		virtual void onFrameEnd( GameEngine* source, float nowSecs, float deltaSecs );

		void setFPSCounter(FPSCounter* counter);

		FPSCounter* getFPSCounter() { return fpsCounter.get(); }

		void setShowFPS(bool b) { showFPS = b; }

		bool isShowingFPS() const { return showFPS; }

	private:
		std::wstring appName;
		xptr<FPSCounter> fpsCounter;
		bool showFPS;
	};
}}

