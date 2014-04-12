#include "LightComponent.h"
#include <irrlicht/ILightSceneNode.h>
#include <irrlicht/ISceneManager.h>
#include <Engine/Properties.h>

namespace xihad { namespace render3d
{
	using namespace ngn;
	using namespace core;
	using std::string;

	LightComponent::LightComponent(string const& name, GameObject& host, ILightSceneNode* node ) :
		RenderComponent(name, host, node)
	{
		node->getLightData().SpecularColor = video::SColorf();
		node->getLightData().Radius = 1000.f;
	}

	LightComponent* LightComponent::create( 
		const string& compName, GameObject& obj, const Properties& param, 
		ISceneManager* smgr )
	{
		ILightSceneNode* lightNode = smgr->addLightSceneNode();
		return new LightComponent(compName, obj, lightNode);
	}

	void LightComponent::setLightData( const video::SLight& light )
	{
		getNode()->setLightData(light);
	}

	video::SLight const& LightComponent::getLightData() const
	{
		return getNode()->getLightData();
	}

	video::SLight& LightComponent::getLightData()
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

	void LightComponent::setLightType( video::E_LIGHT_TYPE type )
	{
		getNode()->setLightType(type);
	}

	video::E_LIGHT_TYPE LightComponent::getLightType() const
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

	ILightSceneNode * LightComponent::getNode() const
	{
		return (ILightSceneNode *) RenderComponent::getNode();
	}
}}

