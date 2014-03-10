#include "WindowEventSeizer.h"
#include "GameWorld.h"
#include "NativeWindow.h"
#include "GameScene.h"
#include "UserEventReceiverStack.h"

namespace xihad { namespace ngn
{
	void WindowEventSeizer::onSceneChanged( GameWorld* world, GameScene* prev )
	{
		if (world->getScene())
			wnd.setEventReceiver(&world->getScene()->getControllerStack());
		else
			wnd.setEventReceiver(0);
	}
}}

