#pragma once
#include <irrlicht/IReferenceCounted.h>

struct lua_State;
namespace xihad { namespace particle
{
	class IStackPusher;
	class IParticleSystemScriptFactory : public virtual irr::IReferenceCounted
	{
	public:
		virtual ~IParticleSystemScriptFactory() {}

		virtual int compositeIniter(lua_State*) = 0;
		virtual IStackPusher* initer(const char* iname)	  = 0;
		virtual IStackPusher* renderer(const char* rname) = 0;
		virtual IStackPusher* affector(const char* aname) = 0;
		virtual IStackPusher* emitter(const char* ename)  = 0;
	};
}}

