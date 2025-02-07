#pragma once
#include "UserEventReceiver.h"
#include <list>
#include "xptr.h"

namespace xihad { namespace ngn
{
	class UserEventReceiverStack : public UserEventReceiver
	{
	public:
		UserEventReceiverStack(bool frontOnly = false);

		virtual ~UserEventReceiverStack();

		//! push a receiver that will have the highest priority to handle event onto stack
		/**
		 * When you create a new UserEventReceiver, you shouldn't drop it. Let those
		 * who pop receiver drop it instead.
		 */
		virtual void pushReceiver(UserEventReceiver*);

		//! Pop the receiver with highest priority to handle event from stack
		/**
		 * The one who pop receiver should take care of receiver dropping.
		 * 
		 * @return popped receiver, which should be a valid pointer
		 */
		virtual xptr<UserEventReceiver> popReceiver();

		UserEventReceiver* frontReceiver() const;

		void setReceiveFrontEventOnly(bool b) { receiveFrontEventOnly = b; }

		bool isReceivingFrontEventOnly() const { return receiveFrontEventOnly; }

		virtual int onKeyEvent(const KeyEvent& event, int argFromPreviousReceiver);

		virtual int onMouseEvent(const MouseEvent& event, int argFromPreviousReceiver);

	private:
		std::list<xptr<UserEventReceiver> > receivers;
		bool receiveFrontEventOnly;
	};
}}

