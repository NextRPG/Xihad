#pragma once
#include "IReferenceCounted.h"
#include "dimension2d.h"

namespace irr { namespace scene
{
	class IAnimatedMeshSceneNode;
	class IMesh;

	// Initializer
	class IBasicParticleInitializer;
	class IParticleAnimatedMeshInitializer;
	class IParticleBoxInitializer;
	class IParticleCylinderInitializer;
	class IParticleDirectionInitializer;
	class IParticleMeshInitializer;
	class IParticleRingInitializer;
	class IParticleSphereInitializer;
	class IParticleCompositeInitializer;

	// Affector
	class IParticleAffector;
	class IParticleAttractionAffector;
	class IParticleFadeOutAffector;
	class IParticleGravityAffector;
	class IParticleRotationAffector;

	// Emitter
	class IParticleEmitter;

	// Renderer
	class IParticleRenderer;
	class IQuadParticleRenderer;

	class IParticleSystemFactory : public virtual IReferenceCounted
	{
	public:
		// Initializer
		virtual IBasicParticleInitializer* createBasicParticleInitializer() = 0;
		virtual IParticleAnimatedMeshInitializer* createAnimatedMeshInitializer() = 0;
		virtual IParticleBoxInitializer* createBoxInitializer() = 0;
		virtual IParticleCylinderInitializer* createCylinderInitializer() = 0;
		virtual IParticleDirectionInitializer* createDirectionInitializer() = 0;
		virtual IParticleMeshInitializer* createMeshInitializer() = 0;
		virtual IParticleRingInitializer* createRingInitializer() = 0;
		virtual IParticleSphereInitializer* createSphereInitializer() = 0;
		virtual IParticleCompositeInitializer* createCompositeInitializer() = 0;

		// Affector
		virtual IParticleAttractionAffector* createAttractionAffector() = 0;
		virtual IParticleFadeOutAffector* createFadeOutAffector() = 0;
		virtual IParticleGravityAffector* createGravityAffector() = 0;
		virtual IParticleRotationAffector* createRotationAffector() = 0;
		virtual IParticleAffector* createScaleAffector(
			const core::dimension2df& scaleTo) = 0;

		// Renderer
		virtual IQuadParticleRenderer* createQuadRenderer() = 0;
		virtual IParticleRenderer* createBillboardRenderer() = 0;
		virtual IParticleRenderer* createDirectionalRenderer() = 0;

		// Emitter
		virtual IParticleEmitter* createEmitter() = 0;
	};
}}

