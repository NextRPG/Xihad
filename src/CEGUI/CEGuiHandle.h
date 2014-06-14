#pragma once

#include <CEGUI/Size.h>
#include <Engine/irr_ptr.h>

struct lua_State;

namespace xihad { namespace ngn
{
	class Timeline;
}}

namespace CEGUI
{
    class IrrlichtEventPusher;
	class Renderer;
	class ImageCodec;
	class ResourceProvider;
	class EventArgs;
	class GUIContext;
	class ScriptModule;
}

namespace irr 
{
	class IrrlichtDevice;
}

namespace xihad { namespace cegui
{
	class CEGUIHandle
	{
	public:
		CEGUIHandle();

		virtual ~CEGUIHandle();

		void initialise(irr::IrrlichtDevice* devece, lua_State* L);

		void cleanup();

		void update(float delta);

		void renderFrame();

		const CEGUI::IrrlichtEventPusher* getEventPusher();

	private:
		void updateCursorPosition();

		void checkWindowResize();

		void initialiseResourceGroupDirectories();

		void initialiseDefaultResourceGroups();

		const char* getDataPathPrefix() const;

	private:
		irr_ptr<irr::IrrlichtDevice> d_device;

		//! size of display last time a change was detected.
		CEGUI::Sizef d_lastDisplaySize;

		CEGUI::Renderer* d_renderer;
		CEGUI::ImageCodec* d_imageCodec;
		CEGUI::ResourceProvider* d_resourceProvider;
		CEGUI::ScriptModule* d_scriptModule;
	};
}}
