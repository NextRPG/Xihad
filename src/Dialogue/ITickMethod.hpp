#pragma once
#include <irrlicht/IReferenceCounted.h>

namespace xihad { namespace dialogue 
{
	class ITickMethod : public virtual irr::IReferenceCounted
	{
	public:	
		virtual ~ITickMethod() {}

		virtual void setTickSpeed(float speed);
		virtual float getTickSpeed();

		virtual void setEventEnabled(bool enable) = 0;
		virtual bool isEventEnabled()  = 0;

		virtual void tick(float delta) = 0;
	};

}}

