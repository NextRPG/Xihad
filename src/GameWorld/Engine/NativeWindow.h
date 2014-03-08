#pragma once
#include "CppBase/ReferenceCounted.h"

namespace xihad { namespace ngn
{
	class UserEventReceiver;
	class WindowRenderer;
	class NativeWindow : public virtual ReferenceCounted
	{
	public:
		virtual void setEventReceiver(UserEventReceiver*) = 0;

		virtual UserEventReceiver* getEventReceiver() = 0;

		virtual WindowRenderer* getRenderer() = 0;

		virtual void handleSystemMessage() = 0;

		virtual bool isClosed() const = 0;

		virtual void setTitle(const wchar_t* title) = 0;

		virtual bool isActive() const = 0;

		//! Get whether the windows is focused.
		/**
		 * Whenever isFocused(), the window isActive()
		 */
		virtual bool isFocused() const = 0;

		virtual bool isFullScreen() const = 0;

		virtual void close() = 0;

		virtual void setResizable(bool) = 0;
	};
}}

