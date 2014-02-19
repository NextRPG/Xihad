#pragma once
#include "ITickEvent.hpp"
#include <Engine/irr_ptr.h>

namespace xihad { namespace dialogue
{
	struct STickEvent 
	{
		unsigned sectionIndex;
		irr_ptr<ITickEvent> event;
	};
}}

