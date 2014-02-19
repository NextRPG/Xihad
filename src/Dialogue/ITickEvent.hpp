#pragma once
#include <irrlicht/IReferenceCounted.h>

namespace xihad { namespace dialogue
{
	class IDialogue;
	class ITickEvent : public virtual irr::IReferenceCounted
	{
	public:
		virtual ~ITickEvent() {}

		virtual void onEventTrigger(IDialogue* eventContext, bool immediateMode) = 0;
	};
}}

