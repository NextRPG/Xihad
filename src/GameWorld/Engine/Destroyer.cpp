#include "Destroyer.h"
#include "Destroyable.h"

namespace xihad { namespace ngn
{
	Destroyer* Destroyer::manageDestroyable( Destroyable* u )
	{
		Destroyer* prevManager = u ? u->getDestroyer() : nullptr;
		if (u && !prevManager)
			u->setDestroyer(this);

		return prevManager;
	}

	Destroyer* Destroyer::unmanageDestroyable( Destroyable* u )
	{
		Destroyer* prevManager = u ? u->getDestroyer() : nullptr;
		if (prevManager == this)
			u->setDestroyer(nullptr);

		return prevManager;
	}

}}

