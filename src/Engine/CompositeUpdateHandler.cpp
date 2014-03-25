#include "CompositeUpdateHandler.h"
#include <algorithm>
#include "CppBase\XiAssert.h"

using namespace std;
namespace xihad { namespace ngn
{

	CompositeUpdateHandler::~CompositeUpdateHandler()
	{
	}

	bool CompositeUpdateHandler::appendChildHandler( UpdateHandler* handler )
	{
		if (!handler) return false;

		xassert(!containsChildHandler(handler));
		mChildHandlerList.push_back(handler);
		if (STARTING<UpdateHandler::status() && UpdateHandler::status()<DEAD)
			handler->start();

		return true;
	}

	void CompositeUpdateHandler::onStart()
	{
		for (iterator it = childHandlerBegin(); it != childHandlerEnd(); ++it)
			(*it)->start();
	}

	void CompositeUpdateHandler::onUpdate( const Timeline& tm )
	{
		UpdateHandler* curr = 0;
		for(iterator iter = childHandlerBegin(); iter != childHandlerEnd();)
		{
			curr = *iter;

			if (curr->status() == DEAD)
			{
				curr->destroy();
				iter = eraseChildHandler(iter);
			} 
			else 
			{
				curr->update(tm);
				 ++iter;
			}
		}
	}

	void CompositeUpdateHandler::onStop()
	{
		for (auto riter = mChildHandlerList.rbegin(); riter != mChildHandlerList.rend(); ++riter)
			(*riter)->stop();
	}

	void CompositeUpdateHandler::onDestroy()
	{
		// Self should be destroyed after all children have been destroyed
		auto riter = mChildHandlerList.rbegin();
		while (riter != mChildHandlerList.rend())
		{
			(*riter)->destroy();
			eraseChildHandler(--riter.base());
		}
	}

	bool CompositeUpdateHandler::destroyChildHandler( UpdateHandler* h )
	{
		const_iterator it = findChildHandler(h);
		if (it != childHandlerEnd())
			if ((*it)->destroy())
			{
				eraseChildHandler(it);
				return true;
			}

		return false;
	}

	CompositeUpdateHandler::const_iterator CompositeUpdateHandler::findChildHandler(UpdateHandler* h) const
	{
		if (!h) return childHandlerEnd();
		return std::find(childHandlerBegin(), childHandlerEnd(), h);
	}

	CompositeUpdateHandler::iterator CompositeUpdateHandler::findChildHandler( UpdateHandler* h )
	{
		if (!h) return childHandlerEnd();
		return std::find(childHandlerBegin(), childHandlerEnd(), h);
	}

	bool CompositeUpdateHandler::containsChildHandler( UpdateHandler* h ) const
	{
		return findChildHandler(h) != childHandlerEnd();
	}

}}

