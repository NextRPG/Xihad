#include "WindowTitleUpdater.h"
#include "FPSCounter.h"
#include "GameEngine.h"
#include "NativeWindow.h"

namespace xihad { namespace ngn
{
	WindowTitleUpdater::WindowTitleUpdater( 
		const std::wstring& appName, FPSCounter* cnter) :
		appName(appName), fpsCounter(cnter) 
	{
		setDebugName("WindowTitleUpdater");
	}

	void WindowTitleUpdater::onFrameBegin( GameEngine* source, float nowSecs )
	{
	}

	void WindowTitleUpdater::onFrameEnd( GameEngine* source, float nowSecs, float deltaSecs )
	{
		std::wstring title = appName;

		if (showFPS)
		{
			title += L"[FPS: (T)]";
			title += std::to_wstring(deltaSecs);
			
			if (fpsCounter)
			{
				title += L", (R)";
				title += std::to_wstring(fpsCounter->getAverageFPS());
			}

			title += L"]";
		}

		source->getWindow()->setTitle(title.c_str());
	}

	WindowTitleUpdater::~WindowTitleUpdater()
	{
	}

	void WindowTitleUpdater::setFPSCounter( FPSCounter* counter )
	{
		fpsCounter = counter;
	}

}}

