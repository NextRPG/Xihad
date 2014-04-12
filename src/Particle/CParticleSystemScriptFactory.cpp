#include "CParticleSystemScriptFactory.h"
#include "CppBase/StdMap.h"
#include "IStackPusherFactory.h"
#include "IrrDelegateStackPusherFactory.h"
#include <irrlicht/IParticleSystemFactory.h>
#include <irrlicht/IParticleCompositeInitializer.h>
#include <irrlicht/IParticleMeshInitializer.h>
#include <irrlicht/IParticleAnimatedMeshInitializer.h>
#include <LuaT/type_checker.h>
#include "CWrappedMeshInitializer.h"
#include "CWrappedAnimatedMeshInitializer.h"
#include "CWrappedIniterStackPusherFactory.h"

namespace xihad { namespace particle
{
	using namespace scene;
	int CParticleSystemScriptFactory::compositeIniter( lua_State* L )
	{
		if (!Factory) return 0;

		IParticleCompositeInitializer* ciniter = Factory->createCompositeInitializer();

		IParticleInitializer* init = ciniter;
		for (int idx = 1; init; ++idx)
		{
			lua_rawgeti(L, -1, idx);
			init = luaT::checkarg<IParticleInitializer*>(L, -1);

			if (init) 
			{
				ciniter->pushBack(init);
				init->drop();
			}
			lua_pop(L, 1);
		}

		luaT::push<IParticleInitializer*>(L, ciniter);
		return 1;
	}

	IStackPusher* CParticleSystemScriptFactory::initer( const char* iname )
	{
		return createFrom(Initers, iname);
	}

	IStackPusher* CParticleSystemScriptFactory::emitter( const char* ename )
	{
		return createFrom(Emitters, ename);
	}

	IStackPusher* CParticleSystemScriptFactory::renderer( const char* rname )
	{
		return createFrom(Renderers, rname);
	}

	IStackPusher* CParticleSystemScriptFactory::affector( const char* aname )
	{
		return createFrom(Affectors, aname);
	}

	IStackPusher* CParticleSystemScriptFactory::createFrom( Registry& r, const std::string& key)
	{
		if (auto got = StdMap::findValuePtr(r, std::string(key)))
			return (*got)->create();

		return 0;
	}


#define DERIVE_DELEGATE(BASE_TYPE, TYPE_NAME) \
	IrrDelegateStackPusherFactory<	\
		IParticle##BASE_TYPE, \
		&IParticleSystemFactory::create##TYPE_NAME>

#define NEW_DERIVE_DELEGATE(BASE_TYPE, TYPE_NAME, f) \
	irrptr<DERIVE_DELEGATE(BASE_TYPE, TYPE_NAME)>(new DERIVE_DELEGATE(BASE_TYPE, TYPE_NAME)(f), false)

#define NEW_DELEGATE(TYPE_NAME, f) \
	NEW_DERIVE_DELEGATE(TYPE_NAME, TYPE_NAME, f)

#define WRAPPED_DELEGATE(WRAP_T, TYPE_NAME)	\
	CWrappedIniterStackPusherFactory<	\
		WRAP_T, IParticle##TYPE_NAME,		\
		&IParticleSystemFactory::create##TYPE_NAME>

#define NEW_WRAPPED_DELEGATE(WRAP_T, TYPE_NAME, f)	\
	irrptr<WRAPPED_DELEGATE(WRAP_T, TYPE_NAME)>(new WRAPPED_DELEGATE(WRAP_T, TYPE_NAME)(f), false)

	IParticleSystemScriptFactory* CParticleSystemScriptFactory::createDefault(
		IParticleSystemFactory* f)
	{
		CParticleSystemScriptFactory* pssf = new CParticleSystemScriptFactory;
		pssf->Factory = f;

		pssf->Initers["Basic"]		 = NEW_DELEGATE(BasicInitializer, f);
		pssf->Initers["Direction"]	 = NEW_DELEGATE(DirectionInitializer, f);
		pssf->Initers["Box"]		 = NEW_DELEGATE(BoxInitializer, f);
		pssf->Initers["Sphere"]		 = NEW_DELEGATE(SphereInitializer, f);
		pssf->Initers["Cylinder"]	 = NEW_DELEGATE(CylinderInitializer, f);
		pssf->Initers["Ring"]		 = NEW_DELEGATE(RingInitializer, f);
		pssf->Initers["Mesh"]		 = NEW_WRAPPED_DELEGATE(CWrappedMeshInitializer, MeshInitializer, f);
		pssf->Initers["AnimatedMesh"]= NEW_WRAPPED_DELEGATE(CWrappedAnimatedMeshInitializer, AnimatedMeshInitializer, f);

		pssf->Affectors["Attraction"]= NEW_DELEGATE(AttractionAffector, f);
		pssf->Affectors["FadeOut"]	 = NEW_DELEGATE(FadeOutAffector, f);
		pssf->Affectors["Force"]	 = NEW_DELEGATE(ForceAffector, f);
		pssf->Affectors["Rotation"]	 = NEW_DELEGATE(RotationAffector, f);

		pssf->Renderers["Quad"]		 = NEW_DELEGATE(QuadRenderer, f);
		pssf->Renderers["Billboard"] = NEW_DERIVE_DELEGATE(QuadRenderer, BillboardRenderer, f);
		pssf->Renderers["Direction"] = NEW_DERIVE_DELEGATE(QuadRenderer, DirectionRenderer, f);
		pssf->Renderers["Point"]	 = NEW_DELEGATE(PointRenderer, f);

		pssf->Emitters[""] = NEW_DELEGATE(Emitter, f);

		return pssf;
	}
}}

