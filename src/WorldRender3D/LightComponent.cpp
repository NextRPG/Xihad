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

	void LightComponent::setAmbientColor( const video::SColor& color )
	{
		getLightData().AmbientColor = color;
	}

	void LightComponent::setDiffuseColor( const video::SColor& color )
	{
		getLightData().DiffuseColor = color;
	}

	void LightComponent::setSpecularColor( const video::SColor& color )
	{
		getLightData().SpecularColor = color;
	}

	void LightComponent::setAttenuation( const core::vector3df& attenuation )
	{
		getLightData().Attenuation = attenuation;
	}

	void LightComponent::setOuterCone( f32 outer )
	{
		getLightData().OuterCone = outer;
	}

	void LightComponent::setInnerCone( f32 inner )
	{
		getLightData().InnerCone = inner;
	}

	void LightComponent::setFalloff( f32 falloff )
	{
		getLightData().Falloff = falloff;
	}

	video::SLight& LightComponent::getLightData()
	{
		return getNode()->getLightData();
	}

}}

