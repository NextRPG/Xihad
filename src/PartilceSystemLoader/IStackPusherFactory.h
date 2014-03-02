#pragma once
#include <irrlicht/IReferenceCounted.h>

namespace xihad { namespace particle
{
	class IStackPusher;
	class IStackPusherFactory : public virtual irr::IReferenceCounted
	{
	public:
		virtual ~IStackPusherFactory() {}
		virtual IStackPusher* create() = 0;
	};
}}

