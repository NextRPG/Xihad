#pragma once
#include "Engine\BaseVisitor.h"
#include "WorldRender3D\MeshComponent.h"
#include "WorldRender3D\LightComponent.h"
#include "WorldRender3D\CameraComponent.h"
#include "WorldRender3D\BillboardComponent.h"
//#include "WorldRender3D\AnimatedMeshComponent.h"

namespace xihad
{
	namespace render3d
	{
		class AnimatedMeshComponent;
	}
}

struct lua_State;
namespace xihad { namespace script
{
	class LuaComponent;
	class PushComponentVisitor : public ngn::BaseVisitor, 
		public ngn::Visitor<script::LuaComponent>,
		public ngn::Visitor<render3d::MeshComponent>,
		public ngn::Visitor<render3d::AnimatedMeshComponent>,
		public ngn::Visitor<render3d::LightComponent>,
		public ngn::Visitor<render3d::CameraComponent>,
		public ngn::Visitor<render3d::BillboardComponent>
	{
	public:
		explicit PushComponentVisitor(lua_State* L) : L(L)
		{
		}

		virtual void visit( render3d::AnimatedMeshComponent& c);
		virtual void visit( render3d::MeshComponent& c);
		virtual void visit( render3d::LightComponent& c);
		virtual void visit( render3d::CameraComponent& c);
		virtual void visit( render3d::BillboardComponent& c);
		virtual void visit( script::LuaComponent& c);

		virtual bool handleUnkownVisitable(ngn::BaseVisitable& c);

	private:
		template <typename T>
		inline void genericVisit(T& c)
		{
			::luaT::push<T*>(L, &c);
		}

		lua_State* L;
	};
}}

