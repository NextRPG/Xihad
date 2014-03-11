#pragma once
#include "Engine\BaseComponentSystem.h"
#include <boost\scoped_ptr.hpp>
#include "AnimationClipsCache.h"

struct lua_State;
namespace irr
{
	class IrrlichtDevice;
	namespace scene
	{
		class ISceneManager;
		class IMesh;
	}
}

namespace xihad { namespace render3d
{
	class MeshManager;
	class TextureManager;
	class IrrlichtComponentSystem : public ngn::BaseComponentSystem
	{
	public:
		IrrlichtComponentSystem(
			irr::IrrlichtDevice* device, irr::scene::ISceneManager* scene,
			const ngn::InheritanceTree& tree, AnimationClipsCache& gCache);

		virtual ~IrrlichtComponentSystem();

		virtual ngn::Component* create(
			const std::string& compName, 
			ngn::GameObject& obj, 
			const ngn::Properties& param = ngn::NullProperties()) override;

		irr::scene::ISceneManager* getSceneManager();

		TextureManager* getTextureManager();

		MeshManager* getMeshManager();

		void setShadowColor();

		void setAmbientColor();

		void setViewport();

	protected:
		virtual void onStart() override;
		virtual void onUpdate( const ngn::Timeline& ) override;
		virtual void onStop() override;

	private:
		struct impl;
		boost::scoped_ptr<impl> mImpl;
	};
}}

