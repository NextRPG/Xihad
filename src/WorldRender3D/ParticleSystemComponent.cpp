#include "ParticleSystemComponent.h"
#include <irrlicht/IParticleSystemSceneNode.h>
#include <Engine/Properties.h>
#include <irrlicht/ISceneManager.h>

namespace xihad { namespace render3d
{
	using namespace scene;
	ParticleSystemComponent::ParticleSystemComponent( const std::string& name, 
		ngn::GameObject& host, IParticleSystemSceneNode* node ) :
		RenderComponent(name, host, node)
	{
	}

	ParticleSystemComponent::~ParticleSystemComponent() { }

	ParticleSystemComponent* ParticleSystemComponent::create( 
		const std::string& name, ngn::GameObject& host, 
		const ngn::Properties& params, ISceneManager* smgr )
	{
		return new ParticleSystemComponent(name, host, smgr->addParticleSystemSceneNode());
	}

	IParticleSystemSceneNode* ParticleSystemComponent::getNode() const
	{
		return (IParticleSystemSceneNode*) RenderComponent::getNode();
	}


	
}}

