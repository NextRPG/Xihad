#include <irrlicht/ISceneCollisionManager.h>
#include <Engine/GameScene.h>
#include <Engine/GameObject.h>
#include <boost/cast.hpp>
#include <luaT/luaT.h>

#include "LuaGlobalVariable.h"
#include "WorldRender3D/RenderComponent.h"

using namespace luaT;
using namespace irr::scene;
using namespace irr::core;
using namespace boost;
namespace xihad { namespace render3d
{
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

	int luaopen_SceneCollisionManager(lua_State* L)
	{
		// collision
		luaT_defRegsBgn(collRegs)
			luaT_cnamedfunc(getRayFromScreenCoord),
			luaT_lnamedfunc(getScreenCoordFromPosition),
			luaT_lnamedfunc(detect),
		luaT_defRegsEnd
		MetatableFactory<ISceneCollisionManager>::create(L, collRegs);

		return 0;
	}
}}

