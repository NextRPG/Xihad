#pragma once
#include "Engine\BaseVisitor.h"
// #include "WorldRender3D\ParticleSystemComponent.h"
// #include "WorldRender3D\MeshComponent.h"
// #include "WorldRender3D\LightComponent.h"
// #include "WorldRender3D\CameraComponent.h"
// #include "WorldRender3D\BillboardComponent.h"
//#include "WorldRender3D\AnimatedMeshComponent.h"

namespace xihad
{
	namespace render3d
	{
		class MeshComponent;
		class LightComponent;
		class CameraComponent;
		class BillboardComponent;
		class TextComponent;
		class AnimatedMeshComponent;
		class ParticleSystemComponent;
	}

	namespace audio
	{
		class AudioComponent;
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
		public ngn::Visitor<render3d::BillboardComponent>,
		public ngn::Visitor<render3d::TextComponent>,
		public ngn::Visitor<render3d::ParticleSystemComponent>,
		public ngn::Visitor<audio::AudioComponent>
	{
	public:
		explicit PushComponentVisitor(lua_State* L) : L(L)
		{
		}

		virtual void visit(audio::AudioComponent&);

		virtual void visit(render3d::AnimatedMeshComponent&);
		virtual void visit(render3d::MeshComponent&);
		virtual void visit(render3d::LightComponent&);
		virtual void visit(render3d::CameraComponent&);
		virtual void visit(render3d::BillboardComponent&);
		virtual void visit(render3d::TextComponent&);
		virtual void visit(render3d::ParticleSystemComponent&);

		virtual void visit(script::LuaComponent&);

		virtual bool handleUnkownVisitable(ngn::BaseVisitable&);

	private:
		template <typename T>
		inline void genericVisit(T& c)
		{
			::luaT::push<T*>(L, &c);
		}

		lua_State* L;
	};
}}

