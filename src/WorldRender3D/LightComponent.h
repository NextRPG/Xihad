#pragma once
#include "RenderComponent.h"

namespace irr { namespace video
{
	struct SLight;
	enum E_LIGHT_TYPE;
}}

namespace irr { namespace scene {
	class ILightSceneNode;
}}

namespace xihad { namespace render3d
{
	class LightComponent : public RenderComponent
	{
	public:
		DEFINE_VISITABLE

		LightComponent(std::string const& name, ngn::GameObject& host, ILightSceneNode* node);

		static LightComponent* create(
			const std::string& compName, ngn::GameObject& obj, const ngn::Properties& param, 
			ISceneManager* smgr);

		void setLightData(const video::SLight& light);

		video::SLight const& getLightData() const;

		video::SLight& getLightData();

		void setRadius(float radius);

		float getRadius() const;

		void setLightType(video::E_LIGHT_TYPE type);

		video::E_LIGHT_TYPE getLightType() const;

		void enableCastShadow(bool shadow = true);

		bool getCastShadow() const;

	protected:
		ILightSceneNode * getNode() const;
	};
}}

