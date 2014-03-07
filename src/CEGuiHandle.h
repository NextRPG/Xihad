#pragma once

#if defined(_WIN32)
#  pragma comment(lib, "irrlicht.lib")
#  if defined(_DEBUG)
#      pragma comment(lib, "CEGUIIrrlichtRenderer_d.lib")
#  else
#      pragma comment(lib, "CEGUIIrrlichtRenderer.lib")
#  endif
#endif

#include <limits.h>
#ifdef _MAX_PATH
#define PATH_MAX _MAX_PATH
#else
#define PATH_MAX 260
#endif

#include "CEGUI/Size.h"

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

class CeguiHandle
{
public:
    CeguiHandle();
    virtual ~CeguiHandle() {}

	void initialise(irr::IrrlichtDevice* devece, lua_State* L, const char* scriptFile = nullptr);
	
	void cleanup();

	void update(const xihad::ngn::Timeline& tl);

	void renderFrame();

	const CEGUI::IrrlichtEventPusher* getEventPusher();
private:
	void checkWindowResize();

	void initialiseResourceGroupDirectories();

	void initialiseDefaultResourceGroups();

	const char* getDataPathPrefix() const;
private:
    irr::IrrlichtDevice*       d_device;

    //! size of display last time a change was detected.
    CEGUI::Sizef d_lastDisplaySize;

	CEGUI::Renderer* d_renderer;
	CEGUI::ImageCodec* d_imageCodec;
	CEGUI::ResourceProvider* d_resourceProvider;
	CEGUI::ScriptModule* d_scriptModule;
};
