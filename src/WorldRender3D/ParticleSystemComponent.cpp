#include "ParticleSystemComponent.h"
#include <irrlicht/IParticleSystemSceneNode.h>
#include <Engine/Properties.h>

namespace xihad { namespace render3d
{
	using namespace scene;
	ParticleSystemComponent::ParticleSystemComponent( const std::string& name, 
		ngn::GameObject& host, IParticleSystemSceneNode* node ) :
		RenderComponent(name, host, node)
	{
	}

	ParticleSystemComponent* ParticleSystemComponent::create( 
		const std::string& name, ngn::GameObject& host, 
		const ngn::Properties& params, ISceneManager* smgr )
	{
		const char* effectPath = params.getString("effectFile");
		params.getString("dst");
		// gameScene->findObject(src);
		// gameScene->findObject(dst);
		// lua_
	}

	IParticleSystemSceneNode* ParticleSystemComponent::getNode() const
	{
		return (IParticleSystemSceneNode*) RenderComponent::getNode();
	}

	ParticleSystemComponent::~ParticleSystemComponent()
	{
	}

	
}}

