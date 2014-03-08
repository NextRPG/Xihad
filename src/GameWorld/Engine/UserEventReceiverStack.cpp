#include "UserEventReceiverStack.h"

namespace xihad { namespace ngn
{

	UserEventReceiverStack::UserEventReceiverStack(bool frontOnly) : 
		receiveFrontEventOnly(frontOnly) { }

	void UserEventReceiverStack::pushReceiver( UserEventReceiver* recv )
	{
		if (recv)
		{
			assert(recv->getReferenceCount() >= 1 && 
				"When you create a new UserEventReceiver, you shouldn't drop it.");
			receivers.push_back(recv);
		}
	}

	UserEventReceiver* UserEventReceiverStack::popReceiver()
	{
		UserEventReceiver* back = receivers.back().get();
		assert(back->getReferenceCount() >= 1);
		receivers.pop_back();
		return back;
	}

	int UserEventReceiverStack::onKeyEvent( const KeyEvent& event, int argFromPreviousReceiver )
	{
		if (receiveFrontEventOnly && argFromPreviousReceiver!=0) 
			return argFromPreviousReceiver;

		int arg = argFromPreviousReceiver;
		for (irr_ptr<UserEventReceiver> recv : receivers)
		{
			arg = recv->onKeyEvent(event, arg);

			if (arg == 0) break;
		}

		return arg;
	}

	int UserEventReceiverStack::onMouseEvent( const MouseEvent& event, int argFromPreviousReceiver )
	{
		if (receiveFrontEventOnly && argFromPreviousReceiver!=0)
			return argFromPreviousReceiver;

		int arg = argFromPreviousReceiver;
		for (irr_ptr<UserEventReceiver> recv : receivers)
		{
			arg = recv->onMouseEvent(event, arg);

			if (arg == 0) break;
		}

		return arg;
	}

}}

