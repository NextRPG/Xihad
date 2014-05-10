#pragma once
#include "CppBase/ReferenceCounted.h"
#include "dimension2d.h"

namespace xihad { namespace ngn
{
	class UserEventReceiver;
	class WindowRenderer;
	class NativeWindow : public virtual ReferenceCounted
	{
	public:
		NativeWindow() { XIHAD_MLD_NEW_OBJECT; }

		virtual ~NativeWindow() { XIHAD_MLD_DEL_OBJECT; }

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

		virtual const dimension2du& getScreenSize() const = 0;
	};
}}

