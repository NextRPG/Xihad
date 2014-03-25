#pragma once
#include <string>
#include <unordered_map>

namespace xihad { namespace ngn
{
	class ComponentSystemFactory;
	class ComponentSystemRegistry
	{
	public:
		static bool registerSystem(
			const std::string& systemName, ComponentSystemFactory* sytem);

		static ComponentSystemFactory* findSystem(const std::string& systemName);

		static ComponentSystemFactory* setDefaultFactory(ComponentSystemFactory* defaultFactory);

		static ComponentSystemFactory* getDefaultFactory();

		static void destroy();

	private:
		typedef std::unordered_map<std::string, ComponentSystemFactory*> Registry; 
		static Registry sFactoryRegistry;
		static ComponentSystemFactory* sDefaultFactory;
	};
}}

