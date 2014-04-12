#pragma once
#include "Engine/UpdateHandler.h"
#include "Engine/xptr.h"

namespace xihad { namespace ngn
{
	class UserEventReceiver;
}}

namespace xihad { namespace cegui
{
	class CEGUIHandle;
	class CEGUIUpdateHandler : public ngn::UpdateHandler
	{
	public:
		explicit CEGUIUpdateHandler(CEGUIHandle* handle);

		ngn::UserEventReceiver* getEventReceiver() { return eventReceiver.get(); }

	protected:
		virtual ~CEGUIUpdateHandler();

		virtual void onStart();

		virtual void onUpdate(const ngn::Timeline&);

		virtual void onStop();

	private:
		CeguiHandle* ceguiHandle;
		xptr<ngn::UserEventReceiver> eventReceiver;
	};
}}

