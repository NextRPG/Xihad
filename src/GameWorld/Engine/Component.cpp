#include "Component.h"
#include <map>
#include <list>
#include "GameObject.h"

namespace xihad { namespace ngn
{
	struct Component::impl
	{
		std::string compName;
		GameObject* host;
		std::list<Destroyable*> managedDestroyables;
	};
	

	Component::Component( const std::string& typeName, GameObject& host ) : 
		mImpl(new impl)
	{
		mImpl->compName = typeName;
		mImpl->host = &host;
	}

	Component::~Component()
	{
	}

	const std::string& Component::getComponentName() const
	{
		return mImpl->compName;
	}

	GameObject* Component::getHostObject() const
	{
		return mImpl->host;
	}

	void Component::onChildDestroy( Destroyable* toDestroty )
	{
		// TODO
	}

	bool Component::appendDestroyable( Destroyable* u )
	{
		if (u && Destroyer::manageDestroyable(u) == nullptr)
		{
			mImpl->managedDestroyables.push_back(u);
			return true;
		}

		return u && u->getDestroyer() == this;
	}

	bool Component::removeDestroyable( Destroyable* u )
	{
		if (unmanageDestroyable(u) == this)
		{
			mImpl->managedDestroyables.remove(u);
			return true;
		}
		else
		{
			return !u || u->getDestroyer() == nullptr;
		}
	}

	void Component::onDestroy()
	{
		auto& dtors = mImpl->managedDestroyables;

		auto iter = dtors.begin();
		while(iter != dtors.end())
		{
			Destroyable* d = *iter;
			Destroyer* pre = unmanageDestroyable(d);
			xassert(pre == this);
			iter = dtors.erase(iter);

			d->destroy();
		}
	}

}}


