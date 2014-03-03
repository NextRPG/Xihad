#pragma once
#include <irrlicht\IReferenceCounted.h>

struct lua_State;
namespace xihad { namespace particle
{
	class IStackPusher : public virtual irr::IReferenceCounted
	{
	public:	
		virtual ~IStackPusher() {}
		virtual void pushOnto(lua_State*) = 0;
	};
}}	 

