#include "CompositeUpdateHandler.h"
#include <algorithm>
#include "CppBase\XiAssert.h"
#include "boost\cast.hpp"
#include "Timeline.h"

using namespace std;
namespace xihad { namespace ngn
{
	bool CompositeUpdateHandler::appendUpdateHandler( UpdateHandler* handler )
	{
		Destroyer* prev = manageDestroyable(handler);
		if (handler && prev == nullptr)
		{
			xassert(containsUpdateHandler(handler));
			mUpdateImpls.push_back(handler);

			if (STARTING<status() && status()<DEAD)
				handler->start();

			return true;
		}
		else
		{
			return prev == this;
		}
	}

	bool CompositeUpdateHandler::removeUpdateHandler( UpdateHandler* handler )
	{
		Destroyer* prev = unmanageDestroyable(handler);
		if (prev == this)
		{
			auto got = iteratorFor(handler);
			xassert(got != mUpdateImpls.end());
			mUpdateImpls.erase(got);
			return true;
		}
		else
		{
			return prev == nullptr;
		}
	}

	bool CompositeUpdateHandler::containsUpdateHandler( UpdateHandler* handler ) const
	{
		return handler != nullptr && handler->getDestroyer() == this;
	}

	std::list<UpdateHandler*>::const_iterator CompositeUpdateHandler::iteratorFor( UpdateHandler* handler ) const
	{
		return find(mUpdateImpls.begin(), mUpdateImpls.end(), handler);
	}

	void CompositeUpdateHandler::onStart()
	{
		for(auto updater : mUpdateImpls)
			updater->start();
	}

	void CompositeUpdateHandler::onUpdate( const Timeline& tm )
	{
		for(auto updater : mUpdateImpls)
			updater->update(tm);
	}

	void CompositeUpdateHandler::onStop()
	{
		for (auto riter = mUpdateImpls.rbegin(); riter != mUpdateImpls.rend(); ++riter)
			(*riter)->stop();
	}

	bool CompositeUpdateHandler::isDestroying()
	{
		return ManagedUpdateHandler::isDestroying();
	}

	void CompositeUpdateHandler::onChildDestroy( Destroyable* toDestroty )
	{
		if (toDestroty->getDestroyer() == this)
			removeUpdateHandler(boost::polymorphic_downcast<UpdateHandler*>(toDestroty));
	}

	void CompositeUpdateHandler::onDestroy()
	{
		auto& dtors = mUpdateImpls;

		// Self should be destroyed after all children have been destroyed
		auto riter = dtors.rbegin();
		while (riter != dtors.rend())
		{
			Destroyable* d = *riter;
			Destroyer* pre = unmanageDestroyable(d);
			assert(pre == this);
			mUpdateImpls.erase(--riter.base());

			d->destroy();
		}
	}
}}

