#include "LuaAnimationCallback.h"
#include "LuaUtil.h"
#include <iostream>
#include "LuaT\stack_ops.h"
#include "WorldRender3D\RenderComponent.h"
#include "WorldRender3D\AnimatedMeshComponent.h"
#include <boost\cast.hpp>
#include "luaT\stack_memo.h"

using namespace xihad::render3d;
namespace xihad { namespace script
{
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

