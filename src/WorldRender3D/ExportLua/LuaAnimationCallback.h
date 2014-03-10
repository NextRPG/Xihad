#pragma once
#include "irrlicht\IAnimatedMeshSceneNode.h"
#include "LuaT\lua_ref.h"

namespace xihad { namespace render3d
{
	class LuaAnimationCallback : public irr::scene::IAnimationEndCallBack
	{
	public:
		LuaAnimationCallback(const luaT::LuaRef& ref);

		virtual void OnAnimationEnd( 
			irr::scene::IAnimatedMeshSceneNode* node ) override;

	private:
		luaT::LuaRef mCallable;
	};
}}

