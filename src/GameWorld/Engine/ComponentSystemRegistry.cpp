#include "ComponentSystemRegistry.h"
#include "ComponentSystemFactory.h"
#include "ComponentSystem.h"
#include "CppBase\StdMap.h"
#include <set>

namespace xihad { namespace ngn
{
	bool ComponentSystemRegistry::registerSystem( 
		const std::string& compName, ComponentSystemFactory* factory )
	{
		if (factory != NULL && 
			!StdMap::containsKey(sFactoryRegistry, compName))
		{
			sFactoryRegistry[compName] = factory;
			return true;
		}

		return false;
	}

	ComponentSystemFactory* ComponentSystemRegistry::findSystem( const std::string& systemName )
	{
		auto got = StdMap::findPtrValue(sFactoryRegistry, systemName);
		got = (got==nullptr ? sDefaultFactory : got);
		return got;
	}

	ComponentSystemFactory* ComponentSystemRegistry::setDefaultFactory( ComponentSystemFactory* defaultFactory )
	{
		auto prev = sDefaultFactory;
		sDefaultFactory = defaultFactory;
		return sDefaultFactory;
	}

	ComponentSystemFactory* ComponentSystemRegistry::getDefaultFactory()
	{
		return sDefaultFactory;
	}

	void ComponentSystemRegistry::destroy()
	{
		std::set<ComponentSystemFactory*> allFactory;
		for (auto& kv : sFactoryRegistry)
			allFactory.insert(kv.second);
		allFactory.insert(sDefaultFactory);

		for (auto factory : allFactory)
			delete factory;
	}

	ComponentSystemFactory* ComponentSystemRegistry::sDefaultFactory = NULL;

	std::unordered_map<std::string, ComponentSystemFactory*> ComponentSystemRegistry::sFactoryRegistry;

}}
