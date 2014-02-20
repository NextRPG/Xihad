#pragma once
#include <irrlicht/IReferenceCounted.h>

namespace xihad { namespace dialogue 
{
	class ITickMethod : public virtual irr::IReferenceCounted
	{
	public:	
		virtual ~ITickMethod() {}

		virtual void setTickSpeed(float speed) = 0;
		virtual float getTickSpeed() const = 0;

		virtual void tick(float delta) = 0;
	};

}}

