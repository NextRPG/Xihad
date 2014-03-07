#include "IrrlichtWindow.h"
#include <irrlicht/IEventReceiver.h>
#include <irrlicht/IrrlichtDevice.h>
#include "UserEventReceiver.h"
#include "WindowRenderer.h"

using namespace irr;
namespace xihad { namespace ngn
{
	static IEventReceiver* createReceiverAdapter(IrrlichtWindow* window)
	{
		class EventReceiver : public IEventReceiver
		{
		public:
			EventReceiver(IrrlichtWindow* wnd) : window(wnd) {}

			virtual bool OnEvent(const SEvent& event)
			{
				if (UserEventReceiver* recv = window->getEventReceiver())
				{
					switch (event.EventType)
					{
					case EET_MOUSE_INPUT_EVENT:
						return recv->onMouseEvent(event.MouseInput, 0) == 0;
					case EET_KEY_INPUT_EVENT:
						return recv->onKeyEvent(event.KeyInput, 0) == 0;
					}
				}

				return false;
			}

			IrrlichtWindow* window;
		};

		return new EventReceiver(window);
	}

	static WindowRenderer* createRenderer(video::IVideoDriver* driver)
	{
		class IrrlichtWindowRenderer : public WindowRenderer
		{
		public:
			IrrlichtWindowRenderer(video::IVideoDriver* d) : driver(d) {}

			virtual void clearBuffer()
			{
				driver->beginScene(true, true, video::SColor(255, 192, 168, 100));
			}

			virtual void swapBuffer()
			{
				driver->endScene();
			}

			video::IVideoDriver* driver;
		};

		return new IrrlichtWindowRenderer(driver);
	}

	IrrlichtWindow::IrrlichtWindow(irr::IrrlichtDevice& dev) : 
		device(&dev), closed(false), 
		renderer(createRenderer(dev.getVideoDriver()), false),
		irrlichtEventReceiver(createReceiverAdapter(this))
	{
		dev.setEventReceiver(irrlichtEventReceiver);
	}

	IrrlichtWindow::~IrrlichtWindow()
	{
		if (device->getEventReceiver() == irrlichtEventReceiver)
			device->setEventReceiver(0);

		delete irrlichtEventReceiver;
	}

	void IrrlichtWindow::handleSystemMessage()
	{
		closed = !device->run();
	}

	bool IrrlichtWindow::isClosed() const
	{
		return closed;
	}

	void IrrlichtWindow::setTitle( const wchar_t* title )
	{
		device->setWindowCaption(title);
	}

	bool IrrlichtWindow::isActive() const
	{
		return device->isWindowActive();
	}

	bool IrrlichtWindow::isFocused() const
	{
		return device->isWindowFocused();
	}

	bool IrrlichtWindow::isFullScreen() const
	{
		return device->isFullscreen();
	}

	void IrrlichtWindow::close()
	{
		device->closeDevice();
	}

	void IrrlichtWindow::setResizable( bool b )
	{
		device->setResizable(b);
	}

	void IrrlichtWindow::setEventReceiver( UserEventReceiver* recv )
	{
		receiver = recv;
	}

	UserEventReceiver* IrrlichtWindow::getEventReceiver()
	{
		return receiver.get();
	}

	WindowRenderer* IrrlichtWindow::getRenderer()
	{
		return renderer.get();
	}

}}