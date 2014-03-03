#pragma once
#include <irrlicht/IParticleAnimatedMeshInitializer.h>
#include <irrlicht/smartptr.h>

namespace irr { namespace scene
{
	class ISceneNode;
}}

namespace xihad { namespace	 particle
{
	class CWrappedAnimatedMeshInitializer : public irr::scene::IParticleInitializer
	{
	public:
		CWrappedAnimatedMeshInitializer(irr::scene::IParticleAnimatedMeshInitializer* initer) :
			MeshIniter(initer)
		{
		}

		void initialize(irr::scene::SParticle& p) { MeshIniter->initialize(p); }

		void setMinNormalDirectionSize( irr::f32 minSize ) { MeshIniter->setMinNormalDirectionSize(minSize); }

		void setMaxNormalDirectionSize( irr::f32 maxSize ) { MeshIniter->setMaxNormalDirectionSize(maxSize); }

		void setOutlineOnly(bool only) { MeshIniter->setOutlineOnly(only); }

		void setAnimatedNode(irr::scene::ISceneNode*);

	private:
		irrptr<irr::scene::IParticleAnimatedMeshInitializer> MeshIniter;
	};
}}

