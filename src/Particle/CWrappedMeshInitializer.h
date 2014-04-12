#pragma once
#include <irrlicht/IParticleMeshInitializer.h>
#include <irrlicht/smartptr.h>

namespace irr { namespace scene
{
	class IMesh;
}}

namespace xihad { namespace	 particle
{
	using namespace irr;
	class CWrappedMeshInitializer : public scene::IParticleInitializer
	{
	public:
		CWrappedMeshInitializer(scene::IParticleMeshInitializer* initer) :
			MeshIniter(initer)
		{
		}

		void initialize(scene::SParticle& p) { MeshIniter->initialize(p); }

		void setMinNormalDirectionSize( f32 minSize ) { MeshIniter->setMinNormalDirectionSize(minSize); }

		void setMaxNormalDirectionSize( f32 maxSize ) { MeshIniter->setMaxNormalDirectionSize(maxSize); }

		void setOutlineOnly(bool only) { MeshIniter->setOutlineOnly(only); }

		void setMesh(scene::IMesh*);
		
	private:
		irrptr<scene::IParticleMeshInitializer> MeshIniter;
	};
}}


