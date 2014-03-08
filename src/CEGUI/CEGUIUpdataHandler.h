#pragma once
#include "Engine/UpdateHandler.h"
#include "Engine/irr_ptr.h"

namespace xihad { namespace ngn
{
	class UserEventReceiver;
}}

namespace xihad { namespace cegui
{
	class CeguiHandle;
	class CEGUIUpdateHandler : public ngn::UpdateHandler
	{
	public:
		explicit CEGUIUpdateHandler(CeguiHandle* handle);

		ngn::UserEventReceiver* getEventReceiver() { return eventReceiver.get(); }

	protected:
		virtual ~CEGUIUpdateHandler();

		virtual void onStart();

		virtual void onUpdate(const ngn::Timeline&);

		virtual void onStop();

	private:
		CeguiHandle* ceguiHandle;
		irr_ptr<ngn::UserEventReceiver> eventReceiver;
	};
}}

