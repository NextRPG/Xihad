#include "LightComponent.h"
#include "irrlicht/ILightSceneNode.h"

namespace xihad { namespace render3d
{

	LightComponent::LightComponent( std::string const& name, 
		ngn::GameObject& host, irr::scene::ILightSceneNode* node ) :
		RenderComponent(name, host, node)
	{
		node->getLightData().SpecularColor = irr::video::SColorf();
		node->getLightData().Radius = 1000.f;
	}

	void LightComponent::setLightData( const irr::video::SLight& light )
	{
		getNode()->setLightData(light);
	}

	irr::video::SLight const& LightComponent::getLightData() const
	{
		return getNode()->getLightData();
	}

	irr::video::SLight& LightComponent::getLightData()
	{
		return getNode()->getLightData();
	}

	void LightComponent::setRadius( float radius )
	{
		getNode()->setRadius(radius);
	}

	float LightComponent::getRadius() const
	{
		return getNode()->getRadius();
	}

	void LightComponent::setLightType( irr::video::E_LIGHT_TYPE type )
	{
		getNode()->setLightType(type);
	}

	irr::video::E_LIGHT_TYPE LightComponent::getLightType() const
	{
		return getNode()->getLightType();
	}

	void LightComponent::enableCastShadow( bool shadow )
	{
		getNode()->enableCastShadow(shadow);
	}

	bool LightComponent::getCastShadow() const
	{
		return getNode()->getCastShadow();
	}

	irr::scene::ILightSceneNode * LightComponent::getNode() const
	{
		return (irr::scene::ILightSceneNode *) RenderComponent::getNode();
	}
}}

