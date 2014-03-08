#include "CeguiEventReceiver.h"
#include <CEGUI\System.h>
#include <CEGUI\GUIContext.h>
#include <CEGUI\RendererModules\Irrlicht\EventPusher.h>
#include <windows.h>
#include <stringapiset.h>
#include <iostream>
#include "CEGUIBasedDialogue\DialogueParser.h"
#include "Dialogue\IDialogue.hpp"

CeguiEventReceiver::CeguiEventReceiver(const CEGUI::IrrlichtEventPusher& eventPusher)
	: d_eventPusher(eventPusher)
{

}

CeguiEventReceiver::~CeguiEventReceiver()
{

}

bool CeguiEventReceiver::onForegroundEvent( const xihad::ngn::KeyEvent& event )
{
	if(event.PressedDown)
	{
		return OnKeyDown(event.Key, event.Char, event.Control, event.Shift);
	}
	else
	{
		return OnKeyUp(event.Key, event.Char, event.Control, event.Shift);
	}
}

bool CeguiEventReceiver::onForegroundEvent( const xihad::ngn::MouseEvent& event )
{
	return OnMouse(event.X, event.Y, event.Wheel, event.Event);
}

bool CeguiEventReceiver::onBackgroundEvent( const xihad::ngn::KeyEvent& event )
{
	return onForegroundEvent(event);
}

bool CeguiEventReceiver::onBackgroundEvent( const xihad::ngn::MouseEvent& event )
{
	return onForegroundEvent(event);
}

using xihad::dialogue::DialogueParser;
using xihad::dialogue::IDialogue;
static DialogueParser* parser = nullptr;
static IDialogue* dialog = nullptr;
//----------------------------------------------------------------------------//
bool CeguiEventReceiver::OnKeyDown(irr::EKEY_CODE key, wchar_t wch, bool /*ctrl*/, bool /*shift*/)
{
	bool handled = false;
	CEGUI::Key::Scan ceguiKey = d_eventPusher.getKeyCode(key);
	if (CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr())
	{
		handled = ceguiSystem->getDefaultGUIContext().injectKeyDown(ceguiKey);
		handled = ceguiSystem->getDefaultGUIContext().injectChar(wch) || handled;
	}

	if (ceguiKey == CEGUI::Key::C)
	{
		parser = new DialogueParser;
		parser->initialise();
		dialog = parser->generateDialogue();
	}
	else if (ceguiKey = CEGUI::Key::P)
	{
		delete parser;
	}
	else if (ceguiKey = CEGUI::Key::D)
	{
		delete dialog;
	}
	else if (ceguiKey = CEGUI::Key::U)
	{
		dialog->onUpdate(0.1f);
	}

	return handled;
}

bool CeguiEventReceiver::OnKeyUp(irr::EKEY_CODE key, wchar_t /*wch*/, bool /*ctrl*/, bool /*shift*/)
{
	bool handled = false;
	CEGUI::Key::Scan ceguiKey = d_eventPusher.getKeyCode(key);
	if (CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr())
	{
		return ceguiSystem->getDefaultGUIContext().injectKeyUp(ceguiKey);
	}
	return handled;
}

bool CeguiEventReceiver::OnMouse(irr::s32 x, irr::s32 y, irr::f32 w, irr::EMOUSE_INPUT_EVENT e)
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

