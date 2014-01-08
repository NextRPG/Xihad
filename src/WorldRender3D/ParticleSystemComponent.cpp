#include "ParticleSystemComponent.h"
#include "irrlicht\IParticleSystemSceneNode.h"

using namespace irr::scene;
namespace xihad { namespace render3d
{
	ParticleSystemComponent::ParticleSystemComponent( const std::string& name, 
		ngn::GameObject& host, IParticleSystemSceneNode* node ) :
		RenderComponent(name, host, node)
	{
	}

	IParticleSystemSceneNode* ParticleSystemComponent::getNode() const
	{
		return (IParticleSystemSceneNode*) RenderComponent::getNode();
	}

	ParticleSystemComponent::~ParticleSystemComponent()
	{
	}

	
}}

