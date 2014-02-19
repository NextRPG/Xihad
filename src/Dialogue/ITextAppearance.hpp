#pragma once
#include <Engine/SColor.h>
#include <Engine/irr_ptr.h>
#include <irrlicht/IReferenceCounted.h>

namespace xihad { namespace dialogue
{
	class IMetrics;
	class ITextAppearance : public irr::IReferenceCounted
	{
	public:
		virtual ~ITextAppearance() {}
		virtual IMetrics* getMetrics() const = 0;
	};
}}

