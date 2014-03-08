#ifndef CCEGUIEVENTPUSHER_H_INCLUDED
#define CCEGUIEVENTPUSHER_H_INCLUDED
#include <irrlicht/IEventReceiver.h>
#include <CEGUI/InputEvent.h>
#include "RendererDef.h"

namespace irr { namespace gui 
{
	class ICursorControl;
}}

namespace CEGUI
{
	class IRR_GUIRENDERER_API IrrlichtEventPusher
	{
	public :
		explicit IrrlichtEventPusher(irr::gui::ICursorControl* ctrl);

		virtual ~IrrlichtEventPusher() { }

		bool OnEvent(const irr::SEvent& event);
    
		bool OnKeyDown(irr::EKEY_CODE key, wchar_t wch, bool /*ctrl*/, bool /*shift*/);
    
		bool OnKeyUp(irr::EKEY_CODE key, wchar_t /*wch*/, bool /*ctrl*/, bool /*shift*/);
    
		bool OnMouse(irr::s32 x, irr::s32 y, irr::f32 w, irr::EMOUSE_INPUT_EVENT e);
    
		/*! translate the irrlicht keycode to cegui keycode
		\param kc
		the irrlicht keycode

		\return
		the cegui keycode
		*/
		CEGUI::Key::Scan getKeyCode(irr::EKEY_CODE kc) const;
		
	protected:
		CEGUI::Key::Scan irr2ceCODE[irr::KEY_KEY_CODES_COUNT];

		void initCodes();
   
	private:
		irr::gui::ICursorControl* d_cursorctrl;
	};

}

#endif
