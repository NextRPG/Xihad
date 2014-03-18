#pragma once
#include <irrlicht/IParticleMeshInitializer.h>
#include <irrlicht/smartptr.h>

namespace irr { namespace scene
{
	class IMesh;
}}

namespace xihad { namespace	 particle
{
	class CWrappedMeshInitializer : public irr::scene::IParticleInitializer
	{
	public:
		CWrappedMeshInitializer(irr::scene::IParticleMeshInitializer* initer) :
			MeshIniter(initer)
		{
		}

		void initialize(irr::scene::SParticle& p) { MeshIniter->initialize(p); }

		void setMinNormalDirectionSize( irr::f32 minSize ) { MeshIniter->setMinNormalDirectionSize(minSize); }

		void setMaxNormalDirectionSize( irr::f32 maxSize ) { MeshIniter->setMaxNormalDirectionSize(maxSize); }

		void setOutlineOnly(bool only) { MeshIniter->setOutlineOnly(only); }

		void setMesh(irr::scene::IMesh*);
		
	private:
		irrptr<irr::scene::IParticleMeshInitializer> MeshIniter;
	};
}}


