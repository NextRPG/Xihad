#pragma once
#include "RenderComponent.h"

namespace irr { namespace scene 
{
	class IParticleSystemSceneNode;
}}

namespace xihad { namespace render3d
{
	class ParticleSystemComponent : public RenderComponent
	{
	public:
		DEFINE_VISITABLE

		ParticleSystemComponent(const std::string& name, ngn::GameObject& host, 
			scene::IParticleSystemSceneNode* node);

		static ParticleSystemComponent* create(
			const std::string& name, ngn::GameObject& host, 
			const ngn::Properties& params, ISceneManager* smgr);

		scene::IParticleSystemSceneNode* getParticleNode() const
		{
			return getNode();
		}

	protected:
		~ParticleSystemComponent();

		scene::IParticleSystemSceneNode* getNode() const;
	};
}}
