#pragma once
#include "..\GameWorld\Engine\UserEventReceiver.h"

namespace CEGUI
{
	class IrrlichtEventPusher;
}

class CeguiEventReceiver : public xihad::ngn::UserEventReceiver
{
public:
	explicit CeguiEventReceiver(const CEGUI::IrrlichtEventPusher& eventPusher);

	virtual ~CeguiEventReceiver();

	virtual int onKeyEvent( const xihad::ngn::KeyEvent& event, int argFromPreviousReceiver );

	virtual int onMouseEvent( const xihad::ngn::MouseEvent& event, int argFromPreviousReceiver );

private:
	CeguiEventReceiver(const CeguiEventReceiver&);
	CeguiEventReceiver& operator=(const CeguiEventReceiver&);

	bool CeguiEventReceiver::OnKeyDown(irr::EKEY_CODE key, wchar_t wch, bool ctrl, bool shift);
	
	bool CeguiEventReceiver::OnKeyUp(irr::EKEY_CODE key, wchar_t wch, bool ctrl, bool shift);

	bool CeguiEventReceiver::OnMouse(irr::s32 x, irr::s32 y, irr::f32 w, irr::EMOUSE_INPUT_EVENT e);

private:
	const CEGUI::IrrlichtEventPusher& d_eventPusher;
};

