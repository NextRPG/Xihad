#include "UserEventReceiverStack.h"

namespace xihad { namespace ngn
{

	UserEventReceiverStack::UserEventReceiverStack(bool frontOnly) : 
		receiveFrontEventOnly(frontOnly) 
	{
		XIHAD_MLD_NEW_OBJECT;
	}

	UserEventReceiverStack::~UserEventReceiverStack()
	{
		XIHAD_MLD_DEL_OBJECT;
	}

	void UserEventReceiverStack::pushReceiver( UserEventReceiver* recv )
	{
		if (recv) receivers.push_front(recv);
	}

	UserEventReceiver* UserEventReceiverStack::popReceiver()
	{
		UserEventReceiver* first = receivers.front().get();
		receivers.pop_front();
		first->onStackEvent();
		return first;
	}

	int UserEventReceiverStack::onKeyEvent( const KeyEvent& event, int argFromPreviousReceiver )
	{
		if (receiveFrontEventOnly && argFromPreviousReceiver!=0) 
			return argFromPreviousReceiver;

		int arg = argFromPreviousReceiver;
		for (xptr<UserEventReceiver> recv : receivers)
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
		for (xptr<UserEventReceiver> recv : receivers)
		{
			arg = recv->onMouseEvent(event, arg);

			if (arg == 0) break;
		}

		return arg;
	}

}}

