#include "CompositeUpdateHandler.h"
#include <algorithm>
#include "CppBase\XiAssert.h"
#include "MemoryLeakDetector.h"

using namespace std;
namespace xihad { namespace ngn
{
	CompositeUpdateHandler::CompositeUpdateHandler()
	{
		XIHAD_MLD_NEW_OBJECT;
	}

	CompositeUpdateHandler::~CompositeUpdateHandler()
	{
		XIHAD_MLD_DEL_OBJECT;
	}

	bool CompositeUpdateHandler::appendChildHandler( UpdateHandler* handler )
	{
		if (!handler) return false;

		xassert(!containsChildHandler(handler));
		mChildHandlerList.push_back(handler);
		if (isUpdating()) handler->start();

		return true;
	}

	bool CompositeUpdateHandler::prependChildHandler( UpdateHandler* handler )
	{
		if (!handler) return false;

		xassert(!containsChildHandler(handler));
		mChildHandlerList.push_front(handler);
		if (status() == STARTING || isUpdating()) 
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
		iterator endIter = childHandlerEnd();
		for(iterator iter = childHandlerBegin(); iter != endIter;)
		{
			UpdateHandler* curr = *iter;

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
#ifdef _DEBUG
			size_t sizeBeforeDestroy = mChildHandlerList.size();
			(*riter)->destroy();
			xassert(sizeBeforeDestroy == mChildHandlerList.size() && 
				"UpdateHandler's destroy() function shouldn't remove self from CompositeUpdateHandler's childrenList");
#else
			(*riter)->destroy();
#endif
			eraseChildHandler(--riter.base());
		}

		xassert(mChildHandlerList.empty());
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

