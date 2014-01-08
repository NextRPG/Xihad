#pragma once
#include "UserEvent.h"

namespace xihad { namespace ngn
{
	class UserEventReceiver
	{
	public:
		virtual ~UserEventReceiver() {}
		virtual bool onForegroundEvent(const KeyEvent& event) = 0;
		virtual bool onForegroundEvent(const MouseEvent& event) = 0;
		virtual bool onBackgroundEvent(const KeyEvent& event) = 0;
		virtual bool onBackgroundEvent(const MouseEvent& event) = 0;
	};
}}

