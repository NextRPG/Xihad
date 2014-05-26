#include "ConversationController.h"
#include "Conversation.h"

#include "Engine\UserEvent.h"
#include "Engine\GameScene.h"
#include "Engine\UserEventReceiverStack.h"
#include "Engine\Timeline.h"

namespace xihad { namespace dialogue
{
	using namespace ngn;

	ConversationController::ConversationController(GameScene& scene, xptr<Conversation> conversation)
		: scene(scene), conversation(conversation), autoUnregister(false)
	{
	}

	int ConversationController::onKeyEvent( const KeyEvent& event, int argFromPreviousReceiver )
	{
		Status stat = status();		
		if (!(Status::STARTING>stat || stat<Status::STOPPING))
			return 0;

		if (event.Key == irr::KEY_RETURN && event.PressedDown)
		{
			if (!conversation->nextSpeakPiece())
				conversation->skipSubtitleAnimation();
		}
		else if (event.Key == irr::KEY_SPACE)
		{
			if (event.PressedDown)
				conversation->speedUp();
			else
				conversation->slowDown();
		}

		return 0;
	}

	int ConversationController::onMouseEvent( const MouseEvent& event, int argFromPreviousReceiver )
	{
		return 1;
	}

	void ConversationController::registerToScene(bool autoUnregister /*= true*/)
	{
		this->autoUnregister = autoUnregister;

		if (!scene.containsChildHandler(this))
			scene.appendChildHandler(this);

		UserEventReceiverStack& stack = scene.getControllerStack();
		if (stack.frontReceiver() != this)
			stack.pushReceiver(this);
	}

	void ConversationController::onStart()
	{
		conversation->start();
	}

	void ConversationController::onUpdate( const Timeline& timeline)
	{
		if (!conversation->isFinished())
		{
			conversation->update(timeline.getLastTimeChange());
			return;
		}

		if (autoUnregister)
		{
			UserEventReceiverStack& stack = scene.getControllerStack();
			UserEventReceiver* top = stack.popReceiver().get();
			if (top != this)
				throw std::exception("somebody push UserEventReceiver above ConversationController");

			stop();
		}
	}

	void ConversationController::onStop()
	{
		conversation->stop();
	}

}}

