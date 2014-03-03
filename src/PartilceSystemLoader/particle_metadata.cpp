#include <luaT/luaT.h>
#include "IrrlichtParticleSystems.h"
#include "CWrappedMeshInitializer.h"
#include "CWrappedAnimatedMeshInitializer.h"
#include "IParticleSystemScriptFactory.h"
#include "IParticleSystemLoaderEnv.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace xihad::particle;

luaT_defMetaData(SMaterial, false);
luaT_defMetaData(IParticleInitializer, true);
luaT_defMetaData(IParticleBasicInitializer, false);
luaT_defMetaData(IParticleDirectionInitializer, false);

luaT_defMetaData(IParticleGeometricInitializer, true);
luaT_defMetaData(CWrappedMeshInitializer, false);
luaT_defMetaData(CWrappedAnimatedMeshInitializer, false);
luaT_defMetaData(IParticleBoxInitializer, false);
luaT_defMetaData(IParticleCylinderInitializer, false);
luaT_defMetaData(IParticleRingInitializer, false);
luaT_defMetaData(IParticleSphereInitializer, false);
luaT_defMetaData(IParticleCompositeInitializer, false);

luaT_defMetaData(IParticleAffector, true);
luaT_defMetaData(IParticleAttractionAffector, false);
luaT_defMetaData(IParticleFadeOutAffector, false);
luaT_defMetaData(IParticleForceAffector, false);
luaT_defMetaData(IParticleRotationAffector, false);

luaT_defMetaData(IParticleEmitter, false);

luaT_defMetaData(IParticleRenderer, true);
luaT_defMetaData(IParticleQuadRenderer, false);
luaT_defMetaData(IParticlePointRenderer, false);

luaT_defMetaData(IParticleSystemSceneNode, false);
luaT_defMetaData(IParticleSystemScriptFactory, false);
luaT_defMetaData(IParticleSystemLoaderEnv, false);
luaT_defMetaData(IMesh, false);
luaT_defMetaData(ISceneNode, true);