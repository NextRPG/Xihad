#pragma once
#include <irrlicht/IParticleAnimatedMeshInitializer.h>
#include <irrlicht/smartptr.h>

namespace irr { namespace scene
{
	class ISceneNode;
}}

namespace xihad { namespace	 particle
{
	using namespace irr;
	class CWrappedAnimatedMeshInitializer : public scene::IParticleInitializer
	{
	public:
		CWrappedAnimatedMeshInitializer(scene::IParticleAnimatedMeshInitializer* initer) :
			MeshIniter(initer)
		{
		}

		void initialize(scene::SParticle& p) { MeshIniter->initialize(p); }

		void setMinNormalDirectionSize( f32 minSize ) { MeshIniter->setMinNormalDirectionSize(minSize); }

		void setMaxNormalDirectionSize( f32 maxSize ) { MeshIniter->setMaxNormalDirectionSize(maxSize); }

		void setOutlineOnly(bool only) { MeshIniter->setOutlineOnly(only); }

		void setAnimatedNode(scene::ISceneNode*);

	private:
		irrptr<scene::IParticleAnimatedMeshInitializer> MeshIniter;
	};
}}

