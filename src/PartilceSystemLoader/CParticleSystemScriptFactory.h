#pragma once
#include "IParticleSystemScriptFactory.h"
#include <irrlicht/smartptr.h>
#include <unordered_map>
#include <string>

namespace irr { namespace scene
{
	class IParticleSystemFactory;
}}

namespace xihad { namespace particle
{
	class IStackPusherFactory;
	class CParticleSystemScriptFactory : public IParticleSystemScriptFactory
	{
		typedef std::unordered_map<std::string, irrptr<IStackPusherFactory>> Registry;

	public:
		virtual int compositeIniter(lua_State*);
		virtual IStackPusher* initer(const char* iname);
		virtual IStackPusher* renderer(const char* rname);
		virtual IStackPusher* affector(const char* aname);
		virtual IStackPusher* emitter(const char* ename);

		static IParticleSystemScriptFactory* createDefault(irr::scene::IParticleSystemFactory*);

	private:
		IStackPusher* createFrom(Registry&, const std::string&);

	public:
		irrptr<irr::scene::IParticleSystemFactory> Factory;	// for composite initer
		Registry Initers;
		Registry Emitters;
		Registry Renderers;
		Registry Affectors;
	};
}}
