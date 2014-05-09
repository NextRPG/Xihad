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

		void setAmbientColor(const video::SColor& color);
		void setDiffuseColor(const video::SColor& color);
		void setSpecularColor(const video::SColor& color);
		void setAttenuation(const core::vector3df& attenuation);
		void setOuterCone(f32 outer);
		void setInnerCone(f32 inner);
		void setFalloff(f32 falloff);

		void setRadius(float radius);

		float getRadius() const;

		void setLightType(video::E_LIGHT_TYPE type);

		video::E_LIGHT_TYPE getLightType() const;

		void enableCastShadow(bool shadow = true);

		bool getCastShadow() const;

	private:
		video::SLight& getLightData();

	protected:
		ILightSceneNode * getNode() const;
	};
}}

