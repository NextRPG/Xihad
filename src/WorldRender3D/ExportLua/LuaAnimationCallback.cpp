#include "LuaAnimationCallback.h"
#include <ScriptEngine/LuaUtil.h>
#include <luaT\stack_memo.h>
#include <LuaT\stack_ops.h>
#include <boost\cast.hpp>
#include <iostream>

#include "..\RenderComponent.h"
#include "..\AnimatedMeshComponent.h"

namespace xihad { namespace render3d
{
	using namespace script;

	LuaAnimationCallback::LuaAnimationCallback( const luaT::LuaRef& ref ) :
		mCallable(ref)
	{
		lua_State* L = mCallable.getState();
		luaT::StackMemo memo(L);

		mCallable.pushSelf();
		if (!LuaUtil::iscallable(mCallable.getState(), -1))
			std::cout << "WARNING: AnimationEndCallback should be callable" << std::endl;
	}

	void LuaAnimationCallback::OnAnimationEnd( 
		irr::scene::IAnimatedMeshSceneNode* node )
	{
		lua_State* L = mCallable.getState();
		mCallable.pushSelf();
		RenderComponent* rc = RenderComponent::getComponentFromNode(node);
		auto ac = boost::polymorphic_downcast<AnimatedMeshComponent*> (rc);
		luaT::push<decltype(ac)>(L, ac);
		
		if (lua_pcall(L, 1, 0, 0))
			LuaUtil::outputErrorMessage(L, "AnimationEndCallback");
	}

}}

