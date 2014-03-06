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
	}
}
class CeguiHandle;
namespace xihad { namespace render3d
{
	struct IrrlichtComponentSystemImpl;
	class IrrlichtComponentSystem : public ngn::BaseComponentSystem
	{
	public:
		IrrlichtComponentSystem(
			irr::IrrlichtDevice* device, irr::scene::ISceneManager* scene,
			const ngn::InheritanceTree& tree, AnimationClipsCache& gCache,
			CeguiHandle* ceguiSystem);

		virtual ~IrrlichtComponentSystem();

		virtual ngn::Component* create(const std::string& compName, 
			ngn::GameObject& obj, const ngn::Properties& param = ngn::NullProperties()) override;

		virtual void onStart() override;

		virtual void onUpdate( const ngn::Timeline& ) override;

		virtual void onStop() override;

		irr::scene::ISceneManager* getSceneManager();

	private:
		boost::scoped_ptr<IrrlichtComponentSystemImpl> mImpl;
	};
}}

