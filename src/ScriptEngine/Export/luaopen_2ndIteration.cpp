#include "luaopen_2ndIteration.h"
#include <luaT/luaT.h>
#include <irrlicht/ICursorControl.h>
#include "irrlicht/ISceneCollisionManager.h"
#include "../LuaScriptMacro.h"
#include "Engine/GameScene.h"
#include "WorldRender3D/IrrlichtComponentSystem.h"
#include "boost/cast.hpp"
#include "irrlicht/ISceneManager.h"
#include "irrlicht/ICameraSceneNode.h"
#include "WorldRender3D/RenderComponent.h"
#include "Engine/GameObject.h"
#include "luaopen_GameScene.h"

using namespace luaT;
using namespace irr;
using namespace gui;
using namespace scene;
using namespace core;
using boost::polymorphic_downcast;
using namespace xihad::render3d;

luaT_defMetaData(ICursorControl, false);
luaT_defMetaData(ISceneCollisionManager, false);
luaT_defMetaData(vector3df,	false);
luaT_defMetaData(line3df,	false);
luaT_defMetaData(aabbox3df,	false);

namespace xihad { namespace script
{
	static int getPosition(lua_State* L)
	{
		auto cursor = checkarg<ICursorControl*>(L, 1);
		const position2di& p = cursor->getPosition();
		lua_pushinteger(L, p.X);
		lua_pushinteger(L, p.Y);

		return 2;
	}

	luaT_static line3df getRayFromScreenCoord(ISceneCollisionManager* coll,
		int screenX, int screenY)
	{
		return coll->getRayFromScreenCoordinates(position2di(screenX, screenY));
	}}

	static int getScreenCoordFromPosition(lua_State* L)
	{
		ISceneCollisionManager* coll = checkarg<ISceneCollisionManager*>(L, 1);
		auto pos = checkarg<vector3df*>(L, 2);
		vector2di coord = coll->getScreenCoordinatesFrom3DPosition(*pos);

		lua_pushinteger(L, coord.X);
		lua_pushinteger(L, coord.Y);
		return 2;
	}

	static int detect(lua_State* L)
	{
		ISceneCollisionManager* coll = checkarg<ISceneCollisionManager*>(L, 1);
		line3df* ray = checkarg<line3df*>(L, 2);
		const char* objName = optarg<const char*>(L, 3, nullptr);

		// get global scene
		lua_getglobal(L, LUA_G_SCENE);
		ngn::GameScene* scene = checkarg<ngn::GameScene*>(L, -1);
		lua_pop(L, 1);

		vector3df collPoint;
		triangle3df collTriangle;
		ISceneNode* collNode;
		if (objName == nullptr)
		{
			collNode = coll->getSceneNodeAndCollisionPointFromRay(*ray, collPoint, collTriangle);
			if (collNode != nullptr)
			{
				RenderComponent* comp = RenderComponent::getComponentFromNode(collNode);
				if (comp != nullptr)
				{
					push<ngn::GameObject*>(L, comp->getHostObject());
					push<vector3df>(L, collPoint);
					return 2;
				}
			}
		}
		else if (ngn::GameObject* obj = scene->findObject(objName))
		{
			RenderComponent* rc = polymorphic_downcast<RenderComponent*>(obj->findComponent("Render"));
			if (ITriangleSelector* selector = rc->getTriangleSelector())
			{
				bool detected = coll->getCollisionPoint(*ray, 
					selector, collPoint, collTriangle, collNode);
				if (detected)
				{
					push<vector3df>(L, collPoint);
					return 1;
				}
			}
		}

		return 0;
	}

	int luaopen_2ndIteration(lua_State* L)
	{
		// cursor
		{
			luaT_defRegsBgn(cursorRegs)
				luaT_mnamedfunc_ovl(ICursorControl, void (ICursorControl::*)(float, float), setPosition),
				luaT_lnamedfunc(getPosition),
				luaT_mnamedfunc(ICursorControl, setVisible),
				luaT_mnamedfunc(ICursorControl, isVisible),
			luaT_defRegsEnd
			MetatableFactory<ICursorControl>::create(L, cursorRegs);
		}

		// collision
		{
			luaT_defRegsBgn(collRegs)
				luaT_cnamedfunc(getRayFromScreenCoord),
				luaT_lnamedfunc(getScreenCoordFromPosition),
				luaT_lnamedfunc(detect),
			luaT_defRegsEnd
			MetatableFactory<ISceneCollisionManager>::create(L, collRegs);
		}
		return 0;
	}
}}

