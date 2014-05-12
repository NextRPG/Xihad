#include "LuaAnimationCallback.h"
#include <Engine/MemoryLeakDetector.h>
#include <ScriptEngine/LuaUtil.h>
#include <luaT/stack_memo.h>
#include <LuaT/stack_ops.h>
#include <boost/cast.hpp>
#include <iostream>

#include "../RenderComponent.h"
#include "../AnimatedMeshComponent.h"

namespace xihad { namespace render3d
{
	using namespace script;

	LuaAnimationCallback::LuaAnimationCallback( const luaT::LuaRef& ref ) :
		mCallable(ref)
	{
		XIHAD_MLD_NEW_OBJECT;

		mCallable.pushSelf();
		if (!LuaUtil::iscallable(mCallable.getState(), -1))
		{
			luaT::stackDump(mCallable.getState(), std::cerr);
			std::cerr << "WARNING: AnimationEndCallback should be callable" << std::endl;
		}
		lua_pop(mCallable.getState(), 1);
	}

	LuaAnimationCallback::~LuaAnimationCallback() 
	{
		XIHAD_MLD_DEL_OBJECT;
	}

	void LuaAnimationCallback::OnAnimationEnd( 
		irr::scene::IAnimatedMeshSceneNode* node )
	{
		lua_State* L = mCallable.getState();
		mCallable.pushOnto(L);
		RenderComponent* rc = RenderComponent::getComponentFromNode(node);
		auto ac = boost::polymorphic_downcast<AnimatedMeshComponent*> (rc);
		luaT::push<decltype(ac)>(L, ac);
		
		if (lua_pcall(L, 1, 0, 0))
			LuaUtil::outputErrorMessage(L, "AnimationEndCallback");
	}

}}

