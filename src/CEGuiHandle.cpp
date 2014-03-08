#include "CEGuiHandle.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/System.h>
#include <CEGUI/GUIContext.h>
#include <CEGUI/ScriptModules/Lua/ScriptModule.h>
#include <CEGUIIrrlichtRenderer/ImageCodec.h>
#include <CEGUIIrrlichtRenderer/ResourceProvider.h>
#include <CEGUIIrrlichtRenderer/Renderer.h>
#include <CEGUIIrrlichtRenderer/EventPusher.h>
#include <Engine/Timeline.h>
#include <irrlicht/IrrlichtDevice.h>
#include <limits.h>

#ifdef _MAX_PATH
#define PATH_MAX _MAX_PATH
#else
#define PATH_MAX 260
#endif

namespace xihad { namespace cegui
{
	CeguiHandle::CeguiHandle() :
		d_device(0),
		d_lastDisplaySize(0, 0),
		d_renderer(0),
		d_imageCodec(0),
		d_scriptModule(0),
		d_resourceProvider(0)
	{
	}

	CeguiHandle::~CeguiHandle()
	{
	}

	void CeguiHandle::initialise(irr::IrrlichtDevice* devece, lua_State* L)
	{
		using namespace irr;

		d_device = devece;

		// create irrlicht renderer, image codec and resource provider.
		CEGUI::IrrlichtRenderer& renderer = CEGUI::IrrlichtRenderer::create(*d_device);

		d_renderer = &renderer;
		d_imageCodec = &renderer.createIrrlichtImageCodec(*d_device->getVideoDriver());
		d_resourceProvider =
			&renderer.createIrrlichtResourceProvider(*d_device->getFileSystem());

		d_scriptModule = &CEGUI::LuaScriptModule::create(L);

		// start up CEGUI system using objects created in subclass constructor.
		CEGUI::System::create(*d_renderer, d_resourceProvider, 0, d_imageCodec, d_scriptModule);

		// initialise resource system
		initialiseResourceGroupDirectories();
		initialiseDefaultResourceGroups();

		// clearing this queue actually makes sure it's created(!)
		CEGUI::System::getSingleton().getDefaultGUIContext().clearGeometry(CEGUI::RQ_OVERLAY);
	}


	void CeguiHandle::cleanup()
	{
		CEGUI::System::getSingleton().setScriptingModule(nullptr);
		CEGUI::System::destroy();
		CEGUI::LuaScriptModule::destroy(static_cast<CEGUI::LuaScriptModule&>(*d_scriptModule));

		CEGUI::IrrlichtRenderer& renderer = *static_cast<CEGUI::IrrlichtRenderer*>(d_renderer);
		renderer.destroyIrrlichtResourceProvider(*static_cast<CEGUI::IrrlichtResourceProvider*>(d_resourceProvider));
		renderer.destroyIrrlichtImageCodec(*static_cast<CEGUI::IrrlichtImageCodec*>(d_imageCodec));

		CEGUI::IrrlichtRenderer::destroy(renderer);
		d_device = 0;
	}

	void CeguiHandle::renderFrame()
	{
		CEGUI::System& gui_system(CEGUI::System::getSingleton());

		CEGUI::Renderer* gui_renderer(gui_system.getRenderer());

		gui_renderer->beginRendering();
		gui_system.getDefaultGUIContext().draw();
		gui_renderer->endRendering();

		CEGUI::WindowManager::getSingleton().cleanDeadPool();
	}

	void CeguiHandle::update(float delta)
	{
		void checkWindowResize();

		CEGUI::System& gui_system(CEGUI::System::getSingleton());

		gui_system.injectTimePulse(delta);

		gui_system.getDefaultGUIContext().injectTimePulse(delta);
	}

	void CeguiHandle::checkWindowResize()
	{
		irr::core::dimension2d<irr::u32> cur_size = d_device->getVideoDriver()->getScreenSize();

		if ((static_cast<float>(cur_size.Width) != d_lastDisplaySize.d_width) ||
			(static_cast<float>(cur_size.Height) != d_lastDisplaySize.d_height))
		{
			d_lastDisplaySize.d_width = static_cast<float>(cur_size.Width);
			d_lastDisplaySize.d_height = static_cast<float>(cur_size.Height);
			CEGUI::System::getSingleton().notifyDisplaySizeChanged(d_lastDisplaySize);
		}
	}

	void CeguiHandle::initialiseResourceGroupDirectories()
	{
		// initialise the required dirs for the DefaultResourceProvider
		CEGUI::DefaultResourceProvider* rp =
			static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());

		const char* dataPathPrefix = getDataPathPrefix();
		char resourcePath[PATH_MAX];

		// for each resource type, set a resource group directory
		sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "schemes/");
		rp->setResourceGroupDirectory("schemes", resourcePath);
		sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "imagesets/");
		rp->setResourceGroupDirectory("imagesets", resourcePath);
		sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "fonts/");
		rp->setResourceGroupDirectory("fonts", resourcePath);
		sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "layouts/");
		rp->setResourceGroupDirectory("layouts", resourcePath);
		sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "looknfeel/");
		rp->setResourceGroupDirectory("looknfeels", resourcePath);
		sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "lua_scripts/");
		rp->setResourceGroupDirectory("lua_scripts", resourcePath);
		sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "xml_schemas/");
		rp->setResourceGroupDirectory("schemas", resourcePath);   
		sprintf_s(resourcePath, "%s/%s", dataPathPrefix, "animations/");
		rp->setResourceGroupDirectory("animations", resourcePath); 
	}

	void CeguiHandle::initialiseDefaultResourceGroups()
	{
		// set the default resource groups to be used
		CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
		CEGUI::AnimationManager::setDefaultResourceGroup("animations");

		// setup default group for validation schemas
		CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
		if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
			parser->setProperty("SchemaDefaultResourceGroup", "schemas");
	}

	#define CEGUI_SAMPLE_DATAPATH "assets/datafiles"
	const char* CeguiHandle::getDataPathPrefix() const
	{
		static char dataPathPrefix[PATH_MAX];
		strcpy_s(dataPathPrefix, CEGUI_SAMPLE_DATAPATH);
		return dataPathPrefix;
	}

	const CEGUI::IrrlichtEventPusher* CeguiHandle::getEventPusher()
	{
		CEGUI::IrrlichtRenderer* renderer = static_cast<CEGUI::IrrlichtRenderer*>(d_renderer);
		return renderer == nullptr ? nullptr : renderer->getEventPusher();
	}
}}
