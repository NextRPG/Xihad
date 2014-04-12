#include "CEGUIEventReceiver.h"
#include <CEGUI\System.h>
#include <CEGUI\GUIContext.h>
#include "CEGUIIrrlichtRenderer\EventPusher.h"

using namespace xihad::ngn;

namespace xihad { namespace cegui
{
	CEGUIEventReceiver::CEGUIEventReceiver(const CEGUI::IrrlichtEventPusher& eventPusher)
		: d_eventPusher(eventPusher)
	{

	}

	CEGUIEventReceiver::~CEGUIEventReceiver()
	{

	}

	//----------------------------------------------------------------------------//
	bool CEGUIEventReceiver::OnKeyDown(irr::EKEY_CODE key, wchar_t wch, bool /*ctrl*/, bool /*shift*/)
	{
		bool handled = false;
		CEGUI::Key::Scan ceguiKey = d_eventPusher.getKeyCode(key);
		if (CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr())
		{
			handled = ceguiSystem->getDefaultGUIContext().injectKeyDown(ceguiKey);
			handled = ceguiSystem->getDefaultGUIContext().injectChar(wch) || handled;
		}
		return handled;
	}

	bool CEGUIEventReceiver::OnKeyUp(irr::EKEY_CODE key, wchar_t /*wch*/, bool /*ctrl*/, bool /*shift*/)
	{
		bool handled = false;
		CEGUI::Key::Scan ceguiKey = d_eventPusher.getKeyCode(key);
		if (CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr())
		{
			return ceguiSystem->getDefaultGUIContext().injectKeyUp(ceguiKey);
		}
		return handled;
	}

	bool CEGUIEventReceiver::OnMouse(irr::s32 x, irr::s32 y, irr::f32 w, irr::EMOUSE_INPUT_EVENT e)
	{
		using namespace irr;
		bool handled = false;

		CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr();
		if (!ceguiSystem) return handled;

		switch (e)
		{
			//! Left mouse button was pressed down.
		case EMIE_LMOUSE_PRESSED_DOWN:
			handled = ceguiSystem->getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);
			break;
			//! Right mouse button was pressed down.
		case EMIE_RMOUSE_PRESSED_DOWN:
			handled = ceguiSystem->getDefaultGUIContext().injectMouseButtonDown(CEGUI::RightButton);
			break;
			//! Middle mouse button was pressed down.
		case EMIE_MMOUSE_PRESSED_DOWN:
			handled = ceguiSystem->getDefaultGUIContext().injectMouseButtonDown(CEGUI::MiddleButton);
			break;
			//! Left mouse button was left up.
		case EMIE_LMOUSE_LEFT_UP:
			handled = ceguiSystem->getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);
			break;
			//! Right mouse button was left up.
		case EMIE_RMOUSE_LEFT_UP:
			handled = ceguiSystem->getDefaultGUIContext().injectMouseButtonUp(CEGUI::RightButton);
			break;
			//! Middle mouse button was left up.
		case EMIE_MMOUSE_LEFT_UP:
			handled = ceguiSystem->getDefaultGUIContext().injectMouseButtonUp(CEGUI::MiddleButton);
			break;
			//! The mouse cursor changed its position.
		case EMIE_MOUSE_MOVED:
			handled = ceguiSystem->getDefaultGUIContext().injectMousePosition(
				static_cast<float>(x), static_cast<float>(y));
			break;
			//! The mouse wheel was moved. Use Wheel value in event data to find out
			//! in what direction and how fast.
		case EMIE_MOUSE_WHEEL:
			handled = ceguiSystem->getDefaultGUIContext().injectMouseWheelChange(w);
			break;
		default:
			break;
		}
		return handled;

	}

	int CEGUIEventReceiver::onKeyEvent( const KeyEvent& event, int argFromPreviousReceiver )
	{
		if(event.PressedDown)
			return OnKeyDown(event.Key, event.Char, event.Control, event.Shift) ? 0 : 1;
		else
			return OnKeyUp(event.Key, event.Char, event.Control, event.Shift) ? 0 : 1;
	}

	int CEGUIEventReceiver::onMouseEvent( const MouseEvent& event, int argFromPreviousReceiver )
	{
		return OnMouse(event.X, event.Y, event.Wheel, event.Event) ? 0 : 1;
	}
}}
