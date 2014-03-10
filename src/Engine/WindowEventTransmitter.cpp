#include "WindowEventTransmitter.h"
#include "GameEngine.h"
#include "NativeWindow.h"

namespace xihad { namespace ngn
{

	WindowEventTransmitter::WindowEventTransmitter()
	{
		setDebugName("WindowEventTransmitter");
	}

	void WindowEventTransmitter::onFrameBegin( GameEngine* source, float nowSecs )
	{
		source->getWindow()->handleSystemMessage();

		if (source->getWindow()->isClosed())
			source->stop();
	}

	void WindowEventTransmitter::onFrameEnd( GameEngine* source, float nowSecs, float deltaSecs )
	{
	}

}}
