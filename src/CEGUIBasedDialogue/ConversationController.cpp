#include "ConversationController.h"
#include "Conversation.h"

#include "Engine\UserEvent.h"
#include "Engine\GameScene.h"
#include "Engine\UserEventReceiverStack.h"
#include "Engine\Timeline.h"

namespace xihad { namespace dialogue
{
	using namespace ngn;
	using namespace irr;

	ConversationController::ConversationController(GameScene& scene, irr_ptr<Conversation> conversation)
		: scene(scene), conversation(conversation), autoUnregister(false)
	{

	}

	int ConversationController::onKeyEvent( const KeyEvent& event, int argFromPreviousReceiver )
	{
		Status stat = status();		
		if (!(Status::STARTING>stat || stat<Status::STOPPING))
			return 0;

		if (event.Key == KEY_RETURN && event.PressedDown)
		{
			if (!conversation->nextSpeakPiece())
				conversation->skipSubtitleAnimation();
		}
		else if (event.Key == KEY_SPACE)
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
		UserEventReceiver* top = stack.popReceiver();
		stack.pushReceiver(top);

		if (top != this)
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
			UserEventReceiver* top = stack.popReceiver();
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

