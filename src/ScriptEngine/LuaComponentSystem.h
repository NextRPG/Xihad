#pragma once
#include "Engine/ComponentSystem.h"

struct lua_State;
namespace xihad { namespace ngn
{
	class GameScene;
}}

namespace irr
{
	class IrrlichtDevice;
}

namespace xihad { namespace script
{
	class LuaComponentSystem : public ngn::ComponentSystem
	{
	public:
		LuaComponentSystem(irr::IrrlichtDevice* dev, ngn::GameScene* scene, const std::string& compBase);
		virtual ~LuaComponentSystem();

		ngn::InheritenceChain hierarchy(const std::string& compName) override;

		ngn::Component* create(const std::string& compName, ngn::GameObject& obj, 
			const ngn::Properties& param = ngn::NullProperties()) override;

		bool loadComponent(const std::string& compName);

		lua_State* getLuaState();

	protected:
		virtual void onStart() override;
		virtual void onUpdate(const ngn::Timeline& time) override;
		virtual void onStop() override;

	private:
		bool initComponent(const std::string& compName);
		bool loadBase(std::string& outBase);

	private:
		struct impl;
		impl* mImpl;
	};
}}

