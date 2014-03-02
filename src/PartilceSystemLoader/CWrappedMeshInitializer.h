#pragma once
#include <irrlicht/IParticleGeometricInitializer.h>
#include <irrlicht/smartptr.h>

namespace xihad { namespace	 particle
{
	class CWrappedMeshInitializer : public irr::scene::IParticleInitializer
	{
	public:
		CWrappedMeshInitializer(irr::scene::IParticleGeometricInitializer* initer) :
			MeshIniter(initer)
		{
		}

		void initialize(irr::scene::SParticle& p) { MeshIniter->initialize(p); }

		void setMinNormalDirectionSize( irr::f32 minSize ) { MeshIniter->setMinNormalDirectionSize(minSize); }

		void setMaxNormalDirectionSize( irr::f32 maxSize ) { MeshIniter->setMaxNormalDirectionSize(maxSize); }

		void setOutlineOnly(bool only) { MeshIniter->setOutlineOnly(only); }

		void setMesh(const char* meshDesc);
		
		void setAnimatedMesh(const char* animatedMeshDesc);
		
	private:
		irrptr<irr::scene::IParticleGeometricInitializer> MeshIniter;
	};
}}


