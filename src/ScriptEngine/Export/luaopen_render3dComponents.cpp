#include "luaopen_render3dComponents.h"
#include <luaT/luaT.h>
#include "WorldRender3D/AnimatedMeshComponent.h"
#include "ScriptEngine/LuaAnimationCallback.h"
#include "luaopen_Component.h"
#include "CppBase/StringUtil.h"
#include "WorldRender3D/MeshComponent.h"
#include "WorldRender3D/CameraComponent.h"
#include "Engine/vector3d.h"
#include "WorldRender3D/LightComponent.h"
#include "WorldRender3D/BillboardComponent.h"
#include "irrlicht/SLight.h"

using namespace luaT;
using namespace xihad::render3d;
using namespace xihad::ngn;
using namespace irr::video;
using namespace irr::scene;
luaT_defMetaData(IMesh, true);
luaT_defMetaData(IAnimatedMesh, false);
luaT_defMetaData(RenderComponent, true);
luaT_defMetaData(AnimatedMeshComponent, false);
luaT_defMetaData(MeshComponent, false);
luaT_defMetaData(CameraComponent, false);
luaT_defMetaData(LightComponent, false);
luaT_defMetaData(BillboardComponent, false);

namespace xihad { namespace script
{
	inline static IAnimationEndCallBack* createEndCallback(lua_State* L, int idx)
	{
		LuaRef lcallback = LuaRef::fromIndex(L, idx);
		return new LuaAnimationCallback(lcallback);
	}

	inline static void getLoopAndCallback(lua_State* L, int _1, 
		bool* loop, IAnimationEndCallBack** callback)
	{
		*loop = true;
		*callback = nullptr;
		if (lua_gettop(L) >= _1)
		{
			if (lua_isboolean(L, _1))
			{
				*loop = checkarg<bool>(L, _1);
			}
			else
			{
				*callback = createEndCallback(L, _1);
				*loop = false;
			}
		}
	}

	static int anim_playAnimation(lua_State* L)
	{
		auto animComp = checkarg<AnimatedMeshComponent*>(L, 1);
		int first;

		if (lua_isnumber(L, 2))
		{
			int bgn = checkarg<int>(L, 2);
			int end = checkarg<int>(L, 3);
			animComp->setFrameLoop(bgn, end);
			first = 4;
		}
		else
		{
			std::string& str = checkarg<std::string&>(L, 2);
			animComp->playAnimation(str);
			first = 3;
		}
		
		bool loopMode;
		IAnimationEndCallBack* callback;
		getLoopAndCallback(L, first, &loopMode, &callback);

		animComp->setLoopMode(loopMode);
		animComp->setAnimationEndCallback(callback);

		// carefully drop callback
		if (callback) callback->drop();

		return 0;
	}

	static int anim_setEndCallback(lua_State* L)
	{
		auto animComp = checkarg<AnimatedMeshComponent*>(L, 1);
		IAnimationEndCallBack* callback = createEndCallback(L, 2);
		animComp->setAnimationEndCallback(callback);
		return 0;
	}

	static DebugState idxToEnum[] = { EDS_BBOX, EDS_NORMALS, EDS_SKELETON, 
		EDS_MESH_WIRE_OVERLAY, EDS_HALF_TRANSPARENCY, EDS_BBOX_BUFFERS };
	static DebugState popDebugState(lua_State* L, int idx)
	{
		const char* options[] = { 
			"aabb", "normals", "skeleton", "meshWireOverlay", 
			"halfTransparency", "bboxBuffers", NULL 
		};

		const char* arg = checkarg<const char*>(L, idx);
		int stateIdx = StringUtil::select(arg, options);

		if (stateIdx >= 0)
			return idxToEnum[stateIdx];
		else
			luaL_argerror(L, stateIdx, "No such a debug state");

		return EDS_OFF;
	}

	static int popStates(lua_State* L, int _1)
	{
		int top = lua_gettop(L);
		int state = DebugState::EDS_OFF;
		for (int i = _1; i <= top; ++i)
		{
			int currState = popDebugState(L, i);
			state |= currState;
		}

		return state;
	}

	static int render_addDebugState(lua_State* L)
	{
		RenderComponent* rc = checkarg<RenderComponent*>(L, 1);
		rc->setDebugState(rc->getDebugState() | popStates(L, 2));

		return 0;
	}

	static int render_removeDebugState(lua_State* L)
	{
		RenderComponent* rc = checkarg<RenderComponent*>(L, 1);

		int toClear = popStates(L, 2);
		int mask = ~toClear;
		rc->setDebugState(rc->getDebugState() & mask);

		return 0;
	}

	luaT_static RenderComponent::AABB getAABB(RenderComponent* rc)
	{
		return rc->getBoundingBox();
	}}

	static int render_createTriangleSelector(lua_State* L)
	{
		static SelectorType idxToEnum[] = {
			SelectorType::BoundingBox, Stupid, Octree, Terrian
		};

		const char* options[] = { "aabb", "stupid", "octree", "terrian", NULL };

		RenderComponent* rc = checkarg<RenderComponent*>(L, 1);
		std::string& type = checkarg<std::string&>(L, 2);
		int tIdx = StringUtil::select(type.c_str(), options);

		if (tIdx >= 0)
		{
			SelectorType tEnum = idxToEnum[tIdx];
			int param = (tEnum == Octree) ? 32 : 0;
			if (lua_isnumber(L, 3))
				param = checkarg<int>(L, 3);

			bool ret = rc->createTriangleSelector(SelectorInfo(tEnum, param));
			push<bool>(L, ret);
		}
		else
		{
			luaL_argerror(L, 2, "No such a SelectorType");
		}

		return 1;
	}

	luaT_static void setMaterialTexture(RenderComponent* rc, int textureLayer, const char* texName)
	{
		if (textureLayer < 0) return;

		rc->setMaterialTexture((size_t) textureLayer, texName);
	}}

	luaT_static int getMaterialCount(RenderComponent* rc)
	{
		return (int) rc->getMaterialCount();
	}}

	luaT_static vector3df getTarget(CameraComponent* cc)
	{
		return cc->getTarget();
	}}

	luaT_static vector3df getUpVector(CameraComponent* cc)
	{
		return cc->getUpVector();
	}}

	static int bill_getSize(lua_State* L)
	{
		float h, bw, tw;
		auto bc = checkarg<BillboardComponent*>(L, 1);
		bc->getSize(h, bw, tw);

		push<float>(L, h);
		push<float>(L, bw);
		push<float>(L, tw);
		return 3;
	}

	static int anim_addShadow(lua_State* L)
	{
		auto comp = checkarg<AnimatedMeshComponent*>(L, 1);

		IMesh* shadowMesh = optarg<IMesh*>(L, 2, nullptr);
		const char* method = optarg<const char*>(L, 3, "zfail");
		float inf = optarg<float>(L, 4, 1000.f);
		
		bool zfail = true;
		if (strcmp(method, "zfail") == 0)
			zfail = true;
		else if (strcmp(method, "zpass") == 0)
			zfail = false;
		else 
			luaL_argerror(L, 3, "method accept string 'zfail' or 'zpass'");

		comp->addShadowVolume(shadowMesh, zfail, inf);

		return 0;
	}

	luaT_static void setType(LightComponent* light, const char* type)
	{
		static E_LIGHT_TYPE typeTable[] = { 
			ELT_POINT, ELT_DIRECTIONAL, ELT_SPOT 
		};
		const char* options[] = { "point", "directional", "spot", NULL };

		int idx = StringUtil::select(type, options);
		if (idx >= 0)
		{
			light->setLightType(typeTable[idx]);
		}
	}}

	luaT_static void setColor(RenderComponent* comp, int r, int g, int b)
	{
		for (irr::u32 i = 0; i < comp->getMaterialCount(); ++i)
		{
			SMaterial& mat = comp->getMaterial(i);
			mat.ColorMaterial = false;
			mat.DiffuseColor = SColor(255, r, g, b);
		}
	}}

	int luaopen_render3dComponents( lua_State* L )
	{
		MetatableFactory<IMesh>::createNull(L);
		MetatableFactory<IAnimatedMesh, IMesh>::createNull(L);

		luaT_defRegsBgn(renderRegs)
			luaT_mnamedfunc(RenderComponent, isCulled),
			luaT_mnamedfunc(RenderComponent, setVisible),
			luaT_mnamedfunc(RenderComponent, isVisible),
			luaT_mnamedfunc(RenderComponent, isTrulyVisible),
			luaT_mnnamefunc(RenderComponent, getTransformedBoundingBox, getTransformedBBox),
			luaT_cnamedfunc(getAABB),
			luaT_cnamedfunc(setMaterialTexture),
			luaT_cnamedfunc(getMaterialCount),
			// TODO: Material
			luaT_cnamedfunc(setColor),
			{ "addDebugState", render_addDebugState },
			{ "removeDebugState", render_removeDebugState },
			{ "createSelector", render_createTriangleSelector },
			luaT_mnnamefunc(RenderComponent, removeTriangleSelector, removeSelector),
		luaT_defRegsEnd
		MetatableFactory<RenderComponent, Component>::create(L, renderRegs, 0);

		luaT_defRegsBgn(ameshRegs)
			luaT_mnamedfunc(AnimatedMeshComponent, setMesh),
			luaT_mnamedfunc(AnimatedMeshComponent, getMesh),
			luaT_mnamedfunc(AnimatedMeshComponent, setCurrentFrame),
			luaT_mnamedfunc(AnimatedMeshComponent, setTransitionTime),
			luaT_mnamedfunc(AnimatedMeshComponent, setAnimationSpeed),
			luaT_mnamedfunc(AnimatedMeshComponent, getAnimationSpeed),
			luaT_mnamedfunc(AnimatedMeshComponent, getFrameNumber),
			luaT_mnamedfunc(AnimatedMeshComponent, getStartFrame),
			luaT_mnamedfunc(AnimatedMeshComponent, getEndFrame),
			luaT_mnamedfunc(AnimatedMeshComponent, setLoopMode),
			luaT_mnamedfunc(AnimatedMeshComponent, getLoopMode),
			luaT_lnnamefunc(anim_addShadow, addShadow),
			luaT_lnnamefunc(anim_playAnimation, playAnimation),
			luaT_lnnamefunc(anim_setEndCallback, setEndCallback),
		luaT_defRegsEnd
		MetatableFactory<AnimatedMeshComponent, RenderComponent>::create(L, ameshRegs, 0);

		luaT_defRegsBgn(meshRegs)
			luaT_mnamedfunc(MeshComponent, setMesh),
			luaT_mnamedfunc(MeshComponent, getMesh),
		luaT_defRegsEnd
		MetatableFactory<MeshComponent, RenderComponent>::create(L, meshRegs, 0);

		luaT_defRegsBgn(cameraRegs)
			luaT_mnamedfunc(CameraComponent, activate),
			luaT_mnamedfunc(CameraComponent, isActivating),
			luaT_mnamedfunc(CameraComponent, setTarget),
			luaT_cnamedfunc(getTarget),
			luaT_cnamedfunc(getUpVector),
			luaT_mnamedfunc(CameraComponent, setUpVector),
			luaT_mnamedfunc(CameraComponent, setNearValue),
			luaT_mnamedfunc(CameraComponent, getNearValue),
			luaT_mnamedfunc(CameraComponent, setFarValue),
			luaT_mnamedfunc(CameraComponent, getFarValue),
			luaT_mnamedfunc(CameraComponent, setAspectRatio),
			luaT_mnamedfunc(CameraComponent, getAspectRatio),
			luaT_mnamedfunc(CameraComponent, isOrthogonal),
			luaT_mnamedfunc(CameraComponent, setFOV),
			luaT_mnamedfunc(CameraComponent, getFOV),
		luaT_defRegsEnd
		MetatableFactory<CameraComponent, RenderComponent>::create(L, cameraRegs, 0);

		luaT_defRegsBgn(lightRegs)
			luaT_mnnamefunc(LightComponent, enableCastShadow, castShadow),
			luaT_cnamedfunc(setType),
		luaT_defRegsEnd
		MetatableFactory<LightComponent, RenderComponent>::create(L, lightRegs, 0);

		luaT_defRegsBgn(billRegs)
			luaT_mnamedfunc_ovl(BillboardComponent, 
				void (BillboardComponent::*)(float, float, float), setSize),
				{ "getSize", bill_getSize },
		luaT_defRegsEnd
		MetatableFactory<BillboardComponent, RenderComponent>::create(L, billRegs, 0);
		return 0;
	}
}}

