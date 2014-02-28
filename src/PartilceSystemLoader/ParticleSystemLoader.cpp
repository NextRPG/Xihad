/**
 * TODO 
 * 1. 脚本中进行设置材质
 * 2. 脚本中定义 Affector 和 Initializer
 * 3. 设置 AnimatedMesh 和 Mesh 的参数
 * 4. 重构代码
 * 5. 测试 Texture Region
 * 6. 对几何体初始化类提取基类
 * 7. 添加更多的： normalDirection/fromSurface 方法
 * 8. 测试 AnimatedMesh
 *
 *
 */
#include <map>
#include <string>
#include <luaT/luaT.h>
#include "AllParticleSystem.h"
#include "CppBase/StringUtil.h"

using namespace irr;
using namespace scene;
using namespace luaT;
using namespace xihad;


luaT_defMetaData(IParticleInitializer, true);
luaT_defMetaData(IParticleBasicInitializer, false);
luaT_defMetaData(IParticleAnimatedMeshInitializer, false);
luaT_defMetaData(IParticleBoxInitializer, false);
luaT_defMetaData(IParticleCylinderInitializer, false);
luaT_defMetaData(IParticleDirectionInitializer, false);
luaT_defMetaData(IParticleMeshInitializer, false);
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
luaT_defMetaData(IQuadParticleRenderer, false);
luaT_defMetaData(IPointParticleRenderer, false);

luaT_defMetaData(IParticleSystemSceneNode, false);
luaT_defMetaData(IParticleSystemFactory, false);


static int addEmitter(lua_State* L)
{
	luaT_variable(L, 1, IParticleSystemSceneNode*, pnode);
	luaT_variable(L, 2, IParticleEmitter*, e);

	if (lua_gettop(L) < 3)
		pnode->addEmitter(e);
	else if (lua_gettop(L) < 4)
		pnode->addEmitter(e, (f32)lua_tonumber(L, 3));
	else
		pnode->addEmitter(e, (f32)lua_tonumber(L, 3), (f32)lua_tonumber(L, 4));

	e->drop();
	return 0;
}

static int addAffector(lua_State* L)
{
	luaT_variable(L, 1, IParticleSystemSceneNode*, pnode);
	luaT_variable(L, 2, IParticleAffector*, a);

	if (lua_gettop(L) < 3)
		pnode->addAffector(a);
	else if (lua_gettop(L) < 4)
		pnode->addAffector(a, (f32)lua_tonumber(L, 3));
	else
		pnode->addAffector(a, (f32)lua_tonumber(L, 3), (f32)lua_tonumber(L, 4));

	a->drop();
	return 0;
}

static int initer(lua_State* L)
{
	static const char* initNames[] = {
		"Basic", "Direction", "AnimatedMesh", "Mesh", 
		"Box", "Cylinder", "Ring", "Sphere", NULL
	};

	luaT_variable(L, 1, IParticleSystemFactory*, factory);
	if (!lua_isstring(L, 2))
	{
		if (!lua_istable(L, 2))
		{
			luaL_typerror(L, 2, "Initializer typename / Array of Initializer / table with initialize() function");
			return 0;
		}

		lua_getfield(L, 2, "initialize");
		if (!lua_isnil(L, -1))
		{
			// TODO

			return 1;
		}

		IParticleCompositeInitializer* cinit = factory->createCompositeInitializer();
		int i = 1;
		while (true)
		{
			lua_rawgeti(L, 2, i);
			if (lua_isnil(L, -1))
				break;

			luaT_variable(L, -1, IParticleInitializer*, initer);
			lua_pop(L, 1);
			cinit->pushBack(initer);
			initer->drop();
			++i;
		}

		lua_pop(L, 1);
		push<IParticleCompositeInitializer*>(L, cinit);
		return 1;
	}

	luaT_variable(L, 2, const char*, iname);
	switch (StringUtil::select(iname, initNames))
	{
	case 0:
		push<IParticleBasicInitializer*>(L, factory->createBasicParticleInitializer());
		break;
	case 1:
		push<IParticleDirectionInitializer*>(L, factory->createDirectionInitializer());
		break;
	case 2:
		push<IParticleAnimatedMeshInitializer*>(L, factory->createAnimatedMeshInitializer());
		break;
	case 3:
		push<IParticleMeshInitializer*>(L, factory->createMeshInitializer());
		break;
	case 4:
		push<IParticleBoxInitializer*>(L, factory->createBoxInitializer());
		break;
	case 5:
		push<IParticleCylinderInitializer*>(L, factory->createCylinderInitializer());
		break;
	case 6:
		push<IParticleRingInitializer*>(L, factory->createRingInitializer());
		break;
	case 7:
		push<IParticleSphereInitializer*>(L, factory->createSphereInitializer());
		break;
	default:
		lua_pushnil(L);
		break;
	}

	return 1;
}

static int renderer(lua_State* L)
{
	static const char* renderNames[] = {
		"Quad", "Billboard", "Point", "Direction", NULL
	};

	luaT_variable(L, 1, IParticleSystemFactory*, factory);
	luaT_variable(L, 2, const char*, rname);
	switch (StringUtil::select(rname, renderNames))
	{
	case 0:
		push<IQuadParticleRenderer*>(L, factory->createQuadRenderer());
		break;
	case 1:
		push<IQuadParticleRenderer*>(L, factory->createBillboardRenderer());
		break;
	case 2:
		push<IPointParticleRenderer*>(L, factory->createPointParticleRenderer());
		break;
	case 3:
		push<IQuadParticleRenderer*>(L, factory->createDirectionalRenderer());
		break;
	default:
		lua_pushnil(L);
		break;
	}

	return 1;
}

static int affector(lua_State* L)
{
	static const char* affectorNames[] = {
		"Attraction", "FadeOut", "Force", "Rotation", NULL
	};

	if (lua_istable(L, 2))
	{
		// TODO
		return 1;
	}

	luaT_variable(L, 1, IParticleSystemFactory*, factory);
	luaT_variable(L, 2, const char*, aname);
	switch (StringUtil::select(aname, affectorNames))
	{
	case 0:
		push<IParticleAttractionAffector*>(L, factory->createAttractionAffector());
		break;
	case 1:
		push<IParticleFadeOutAffector*>(L, factory->createFadeOutAffector());
		break;
	case 2:
		push<IParticleForceAffector*>(L, factory->createForceAffector());
		break;
	case 3:
		push<IParticleRotationAffector*>(L, factory->createRotationAffector());
		break;
	default:
		lua_pushnil(L);
		break;
	}

	return 1;
}

static int emitter(lua_State* L)
{
	luaT_variable(L, 1, IParticleSystemFactory*, factory);
	push<IParticleEmitter*>(L, factory->createEmitter());
	return 1;
}

luaT_static void setIniter(IParticleEmitter* emitter,  IParticleInitializer* initer) 
{
	emitter->setParticleInitializer(initer);
	initer->drop();
}}

luaT_static void addTexRegion(IQuadParticleRenderer* quad, f32 x, f32 y, f32 x2, f32 y2)
{
	quad->addTextureRegion(core::rectf(x,y,x2,y2));
}}

luaT_static void grab(IParticleEmitter* e)
{
	e->grab();
}}

luaT_static void setMinColor(IParticleBasicInitializer* init, u32 clr)
{
	init->setMinStartColor(video::SColor(clr));
}}

luaT_static void setMaxColor(IParticleBasicInitializer* init, u32 clr)
{
	init->setMaxStartColor(video::SColor(clr));
}}

luaT_static void setMinWidth(IParticleBasicInitializer* init, f32 minW)
{
	core::dimension2df size = init->getMinStartSize();
	size.Width = minW;
	init->setMinStartSize(size);
}}

luaT_static void setMaxWidth(IParticleBasicInitializer* init, f32 maxW)
{
	core::dimension2df size = init->getMaxStartSize();
	size.Width = maxW;
	init->setMaxStartSize(size);
}}

luaT_static void setMinHeight(IParticleBasicInitializer* init, f32 minH)
{
	core::dimension2df size = init->getMinStartSize();
	size.Height = minH;
	init->setMinStartSize(size);
}}

luaT_static void setMaxHeight(IParticleBasicInitializer* init, f32 maxH)
{
	core::dimension2df size = init->getMaxStartSize();
	size.Height = maxH;
	init->setMaxStartSize(size);
}}


luaT_static void setTargetColor(IParticleFadeOutAffector* faff, u32 clr)
{
	faff->setTargetColor(video::SColor(clr));
}}

luaT_static void setColorMask(IParticleFadeOutAffector* faff, const char* smask)
{
	E_FADEOUT_COLOR_MASK mask = (E_FADEOUT_COLOR_MASK) 0;
	for (int i = 0; i < 4; ++i)
	{
		if (smask[i] == '\0') break;

		switch (smask[i])
		{
		case 'r': case 'R':
			mask = (E_FADEOUT_COLOR_MASK) (mask | EFCM_RED_ENABLE);
			break;
		case 'g': case 'G':
			mask = (E_FADEOUT_COLOR_MASK) (mask | EFCM_GREEN_ENABLE);
			break;
		case 'b': case 'B':
			mask = (E_FADEOUT_COLOR_MASK) (mask | EFCM_BLUE_ENABLE);
			break;
		case 'a': case 'A':
			mask = (E_FADEOUT_COLOR_MASK) (mask | EFCM_ALPHA_ENABLE);
			break;
		default:
			break;
		}
	}

	faff->setColorMask(mask);
}}

namespace {

	template <const char* const rangeField>
	int rangeSetter(lua_State* L)
	{
		std::string setMin = "setMin";
		setMin += rangeField;
		lua_getfield(L, 1, setMin.c_str());
		lua_pushvalue(L, 1);
		lua_pushvalue(L, 2);
		lua_call(L, 2, 0);

		std::string setMax = "setMax";
		setMax += rangeField;
		lua_getfield(L, 1, setMax.c_str());
		lua_pushvalue(L, 1);
		lua_pushvalue(L, 3);
		lua_call(L, 2, 0);

		return 0;
	}

#define DEFINE_FIELD(f) extern char f[] = ###f
#define setRangeFunc(field)	{ "set"###field, rangeSetter<field> }

	DEFINE_FIELD(PPS);
	DEFINE_FIELD(Color);
	DEFINE_FIELD(LifeTime);
	DEFINE_FIELD(Width);
	DEFINE_FIELD(Height);
	DEFINE_FIELD(NormalDirectionSize);
}

int exportLibs(lua_State* L)
{
/////// Particle system scene node
	// Done
	luaT_defRegsBgn(pnode)
		luaT_mnamedfunc(IParticleSystemSceneNode, setParticlesAreGlobal),
		luaT_lnamedfunc(addEmitter),
		luaT_lnamedfunc(addAffector),
		luaT_mnamedfunc(IParticleSystemSceneNode, setRenderer),
	luaT_defRegsEnd
	MetatableFactory<IParticleSystemSceneNode>::create(L, pnode);

/////// Particle system factory
	// Done
	luaT_defRegsBgn(factory)
		luaT_lnamedfunc(initer),
		luaT_lnamedfunc(affector),
		luaT_lnamedfunc(emitter),
		luaT_lnamedfunc(renderer),
	luaT_defRegsEnd
	MetatableFactory<IParticleSystemFactory>::create(L, factory);

/////// Emitter
	// Done
	luaT_defRegsBgn(emitter)
		luaT_cnamedfunc(grab),
		luaT_cnamedfunc(setIniter),
		luaT_mnnamefunc(IParticleEmitter, setMinParticlesPerSecond, setMinPPS),
		luaT_mnnamefunc(IParticleEmitter, setMaxParticlesPerSecond, setMaxPPS),
		setRangeFunc(PPS),
	luaT_defRegsEnd
	MetatableFactory<IParticleEmitter>::create(L, emitter);

/////// Renderer
	// TODO Set Material
	luaT_defRegsBgn(renderer)
	luaT_defRegsEnd
	MetatableFactory<IParticleRenderer>::create(L, renderer);

	// Done
	luaT_defRegsBgn(point_render)
		luaT_mnamedfunc(IPointParticleRenderer, setPointSize),
	luaT_defRegsEnd
	MetatableFactory<IPointParticleRenderer, IParticleRenderer>::create(L, point_render);

	// Done
	luaT_defRegsBgn(quad_render)
		luaT_cnamedfunc(addTexRegion),
		luaT_mnamedfunc(IQuadParticleRenderer, setOrintation),
	luaT_defRegsEnd
	MetatableFactory<IQuadParticleRenderer, IParticleRenderer>::create(L, quad_render);

/////// Initializer
	// Done
	luaT_defRegsBgn(initer)
	luaT_defRegsEnd
	MetatableFactory<IParticleInitializer>::create(L, initer);

	// Done
	luaT_defRegsBgn(basic_initer)
		luaT_cnamedfunc(setMinColor),
		luaT_cnamedfunc(setMaxColor),
		setRangeFunc(Color),
		
		luaT_mnamedfunc(IParticleBasicInitializer, setMinLifeTime),
		luaT_mnamedfunc(IParticleBasicInitializer, setMaxLifeTime),
		setRangeFunc(LifeTime),

		luaT_cnamedfunc(setMinWidth),
		luaT_cnamedfunc(setMaxWidth),
		setRangeFunc(Width),

		luaT_cnamedfunc(setMinHeight),
		luaT_cnamedfunc(setMaxHeight),
		setRangeFunc(Height),
	luaT_defRegsEnd
	MetatableFactory<IParticleBasicInitializer, IParticleInitializer>::create(L, basic_initer);

	// Done
	luaT_defRegsBgn(direction_initer)
		luaT_mnamedfunc(IParticleDirectionInitializer, setMaxAngleDegree),
		luaT_mnamedfunc(IParticleDirectionInitializer, setDirection),
	luaT_defRegsEnd
	MetatableFactory<IParticleDirectionInitializer, IParticleInitializer>::create(L, direction_initer);

	// TODO SET MESH FROM PARENT OR SELF
	luaT_defRegsBgn(amesh_initer)
		luaT_mnamedfunc(IParticleAnimatedMeshInitializer, setMinNormalDirectionSize),
		luaT_mnamedfunc(IParticleAnimatedMeshInitializer, setMaxNormalDirectionSize),
		setRangeFunc(NormalDirectionSize),
	luaT_defRegsEnd
	MetatableFactory<IParticleAnimatedMeshInitializer, IParticleInitializer>::create(L, amesh_initer);

	// TODO SET MESH FROM PARENT OR SELF
	luaT_defRegsBgn(mesh_initer)
		luaT_mnamedfunc(IParticleMeshInitializer, setMinNormalDirectionSize),
		luaT_mnamedfunc(IParticleMeshInitializer, setMaxNormalDirectionSize),
		setRangeFunc(NormalDirectionSize),
	luaT_defRegsEnd
	MetatableFactory<IParticleMeshInitializer, IParticleInitializer>::create(L, mesh_initer);

	// Done
	luaT_defRegsBgn(box_initer)
		luaT_mnamedfunc(IParticleBoxInitializer, setBox),
	luaT_defRegsEnd
	MetatableFactory<IParticleBoxInitializer, IParticleInitializer>::create(L, box_initer);

	// Done
	luaT_defRegsBgn(sph_initer)
		luaT_mnamedfunc(IParticleSphereInitializer, setCenter),
		luaT_mnamedfunc(IParticleSphereInitializer, setRadius),
	luaT_defRegsEnd
	MetatableFactory<IParticleSphereInitializer, IParticleInitializer>::create(L, sph_initer);

	// Done
	luaT_defRegsBgn(cyl_initer)
		luaT_mnamedfunc(IParticleCylinderInitializer, setCenter),
		luaT_mnamedfunc(IParticleCylinderInitializer, setRadius),
		luaT_mnamedfunc(IParticleCylinderInitializer, setNormal),
	luaT_defRegsEnd
	MetatableFactory<IParticleCylinderInitializer, IParticleInitializer>::create(L, cyl_initer);

	// Done
	luaT_defRegsBgn(ring_initer)
		luaT_mnamedfunc(IParticleRingInitializer, setCenter),
		luaT_mnamedfunc(IParticleRingInitializer, setRadius),
		luaT_mnnamefunc(IParticleRingInitializer, setRingThickness, setThickness),
	luaT_defRegsEnd
	MetatableFactory<IParticleRingInitializer, IParticleInitializer>::create(L, ring_initer);

	// Done
	luaT_defRegsBgn(comp_initer)
	luaT_defRegsEnd
	MetatableFactory<IParticleCompositeInitializer, IParticleInitializer>::create(L, comp_initer);

/////// Affector
	// Done
	luaT_defRegsBgn(affector)
	luaT_defRegsEnd
	MetatableFactory<IParticleAffector>::create(L, affector);

	// Done
	luaT_defRegsBgn(att_affector)
		luaT_mnamedfunc(IParticleAttractionAffector, setPoint),
		luaT_mnamedfunc(IParticleAttractionAffector, setMass),
	luaT_defRegsEnd
	MetatableFactory<IParticleAttractionAffector, IParticleAffector>::create(L, att_affector);

	// Done
	luaT_defRegsBgn(fo_affector)
		luaT_cnamedfunc(setTargetColor),
		luaT_cnamedfunc(setColorMask),
	luaT_defRegsEnd
	MetatableFactory<IParticleFadeOutAffector, IParticleAffector>::create(L, fo_affector);
	
	// Done
	luaT_defRegsBgn(for_affector)
		luaT_mnamedfunc(IParticleForceAffector, setGravity),
		luaT_mnamedfunc(IParticleForceAffector, setForce),
	luaT_defRegsEnd
	MetatableFactory<IParticleForceAffector, IParticleAffector>::create(L, for_affector);

	// Done
	luaT_defRegsBgn(rot_affector)
		luaT_mnnamefunc(IParticleRotationAffector, setPivotPoint, setPivot),
		luaT_mnamedfunc(IParticleRotationAffector, setSpeed),
	luaT_defRegsEnd
	MetatableFactory<IParticleRotationAffector, IParticleAffector>::create(L, rot_affector);

	return 0;
}

