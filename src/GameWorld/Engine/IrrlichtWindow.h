#pragma once
#include "NativeWindow.h"
#include <list>
#include "irr_ptr.h"

namespace irr 
{ 
	class IrrlichtDevice; 
	class IEventReceiver;
}

namespace xihad { namespace ngn
{
	class IrrlichtWindow : public NativeWindow
	{
	public:
		explicit IrrlichtWindow(irr::IrrlichtDevice&);	

		irr::IrrlichtDevice* getIrrDevice() { return device.get(); }

		virtual ~IrrlichtWindow();

		virtual void handleSystemMessage();

		virtual bool isClosed() const;

		virtual void setTitle( const wchar_t* title );

		virtual bool isActive() const;

		virtual bool isFocused() const;

		virtual bool isFullScreen() const;

		virtual void close();

		virtual void setResizable( bool ) ;

		virtual void setEventReceiver( UserEventReceiver* );

		virtual UserEventReceiver* getEventReceiver();

		virtual WindowRenderer* getRenderer();

	private:
		irr_ptr<irr::IrrlichtDevice> device;
		irr_ptr<UserEventReceiver> receiver;
		irr_ptr<WindowRenderer> renderer;
		irr::IEventReceiver* irrlichtEventReceiver;
		bool closed;
	};
}}

