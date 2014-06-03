#include "CEGUIUpdataHandler.h"
#include "CEGuiHandle.h"
#include "CeguiEventReceiver.h"
#include "Engine/Timeline.h"

namespace xihad { namespace cegui
{

	CEGUIUpdateHandler::CEGUIUpdateHandler( CEGUIHandle* handle ) :
		ceguiHandle(handle), 
		eventReceiver(new CEGUIEventReceiver(*handle->getEventPusher()), false)
	{
	}

	CEGUIUpdateHandler::~CEGUIUpdateHandler()
	{
		delete ceguiHandle;
	}

	void CEGUIUpdateHandler::onStart()
	{
	}

	void CEGUIUpdateHandler::onUpdate( const ngn::Timeline& tl )
	{
		ceguiHandle->renderFrame();
		ceguiHandle->update(tl.getLastTimeChange());
	}

	void CEGUIUpdateHandler::onStop()
	{

	}

	void CEGUIUpdateHandler::onDestroy()
	{
		ceguiHandle->cleanup();
	}
}}

