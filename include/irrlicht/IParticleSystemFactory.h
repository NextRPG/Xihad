#pragma once
#include "IReferenceCounted.h"
#include "dimension2d.h"

namespace irr { namespace scene
{
	class IAnimatedMeshSceneNode;
	class IMesh;

	// Initializer
	class IParticleBasicInitializer;
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
	class IParticleForceAffector;
	class IParticleRotationAffector;

	// Emitter
	class IParticleEmitter;

	// Renderer
	class IQuadParticleRenderer;
	class IPointParticleRenderer;

	class IParticleSystemFactory : public virtual IReferenceCounted
	{
	public:
		// Initializer
		virtual IParticleBasicInitializer* createBasicParticleInitializer() = 0;
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
		virtual IParticleForceAffector* createForceAffector() = 0;
		virtual IParticleRotationAffector* createRotationAffector() = 0;
		virtual IParticleAffector* createScaleAffector() = 0;

		// Renderer
		virtual IQuadParticleRenderer* createQuadRenderer() = 0;
		virtual IQuadParticleRenderer* createBillboardRenderer() = 0;
		virtual IQuadParticleRenderer* createDirectionalRenderer() = 0;
		virtual IPointParticleRenderer* createPointParticleRenderer() = 0;

		// Emitter
		virtual IParticleEmitter* createEmitter() = 0;
	};
}}

