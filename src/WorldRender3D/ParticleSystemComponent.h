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
			irr::scene::IParticleSystemSceneNode* node);

		static ParticleSystemComponent* create(
			const std::string& name, ngn::GameObject& host, 
			const ngn::Properties& params, ISceneManager* smgr);

	protected:
		~ParticleSystemComponent();

		irr::scene::IParticleSystemSceneNode* getNode() const;
	};
}}
