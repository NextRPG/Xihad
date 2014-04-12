#pragma once
#include "Engine\UserEventReceiver.h"

namespace CEGUI
{
	class IrrlichtEventPusher;
}

namespace xihad { namespace cegui
{
	class CEGUIEventReceiver : public ngn::UserEventReceiver
	{
	public:
		explicit CEGUIEventReceiver(const CEGUI::IrrlichtEventPusher& eventPusher);

		virtual ~CEGUIEventReceiver();

		virtual int onKeyEvent( const ngn::KeyEvent& event, int argFromPreviousReceiver );

		virtual int onMouseEvent( const ngn::MouseEvent& event, int argFromPreviousReceiver );

	private:
		CEGUIEventReceiver(const CEGUIEventReceiver&);
		CEGUIEventReceiver& operator=(const CEGUIEventReceiver&);

		bool CEGUIEventReceiver::OnKeyDown(irr::EKEY_CODE key, wchar_t wch, bool ctrl, bool shift);

		bool CEGUIEventReceiver::OnKeyUp(irr::EKEY_CODE key, wchar_t wch, bool ctrl, bool shift);

		bool CEGUIEventReceiver::OnMouse(irr::s32 x, irr::s32 y, irr::f32 w, irr::EMOUSE_INPUT_EVENT e);

	private:
		const CEGUI::IrrlichtEventPusher& d_eventPusher;
	};
}}
