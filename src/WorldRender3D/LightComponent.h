#pragma once
#include "RenderComponent.h"

namespace irr { 
namespace video
{
	struct SLight;
	enum E_LIGHT_TYPE;
}

namespace scene
{
	class ILightSceneNode;
}
}

namespace xihad { namespace render3d
{
	class LightComponent : public RenderComponent
	{
	public:
		DEFINE_VISITABLE

		LightComponent(std::string const& name, ngn::GameObject& host, 
					   irr::scene::ILightSceneNode* node);

		void setLightData(const irr::video::SLight& light);

		irr::video::SLight const& getLightData() const;

		irr::video::SLight& getLightData();

		void setRadius(float radius);

		float getRadius() const;

		void setLightType(irr::video::E_LIGHT_TYPE type);

		irr::video::E_LIGHT_TYPE getLightType() const;

		void enableCastShadow(bool shadow = true);

		bool getCastShadow() const;

	protected:
		irr::scene::ILightSceneNode * getNode() const;
	};
}}

