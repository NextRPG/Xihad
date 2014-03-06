#include "LuaComponentSystem.h"
#include <Lua/lua.hpp>
#include <LuaT/luaT.h>
#include <luaT/stack_memo.h>
#include <LuaT/metatable.h>
#include <irrlicht/IrrlichtDevice.h>
#include <irrlicht/ISceneCollisionManager.h>
#include <irrlicht/ISceneManager.h>
#include <iostream>
#include "LuaUtil.h"
#include "LuaComponent.h"
#include "Engine/InheritanceTree.h"
#include "Export/luaopen_all.h"
#include "CppBase/XiAssert.h"
#include "Engine/Timeline.h"
#include "LuaScriptMacro.h"
#include "Engine/GameScene.h"
#include "WorldRender3D/Geometry.h"
#include "boost/cast.hpp"
#include "WorldRender3D/IrrlichtComponentSystem.h"

using namespace std;
using namespace luaT;
using namespace irr;
using namespace xihad::ngn;
using namespace xihad::render3d;
namespace xihad { namespace script
{
	struct LuaComponentSystem::impl
	{
		lua_State* L;
		LuaRef lHandle;
		InheritanceTree hierarchy;
	};

	LuaComponentSystem::LuaComponentSystem( IrrlichtDevice* dev,
		GameScene* scene, const std::string& compBase ) :
		mImpl(new impl)
	{
		lua_State* L = mImpl->L = lua_newthread(scene->mainThread());
		mImpl->lHandle = LuaRef::fromTop(scene->mainThread());

		// set path
		{
			StackMemo memo(L);
			lua_getglobal(L, "package");
			string path;
			((path += compBase) += "/") += "?.lua;";
			((path += compBase) += "/") += "?.luac;";
#ifdef _DEBUG
			((path += "Assets/test") += "/") += "?.lua;";
			((path += "Assets/test") += "/") += "?.luac;";
#endif
			setField(L, -1, "path", path.c_str());
			setField(L, -1, "cpath", "./?.dll");
		}
		

		{
			StackMemo memo(L);
			// export cpp library to lua
			luaopen_all(L);
		}

		// set global environment
		{
			StackMemo memo(L);
			// set global variables
			lua_getglobal(L, "_G");

			// _G.scene 
			setField(L, -1, LUA_G_SCENE, scene);

			// _G.Time
			lua_createtable(L, 0, 3);
			{
				// _G.Time.change
				setField(L, -1, LUA_G_TIME_CHANGE, 0);
				// _G.Time.global
				setField(L, -1, LUA_G_TIME_GLOBAL, 0);
				// _G.Time.paused
				setField(L, -1, LUA_G_TIME_PAUSED, false);
			}
			lua_setfield(L, -2, LUA_G_TIME);	// TODO: read-only?

			gui::ICursorControl* cursor = dev->getCursorControl();
			setField(L, -1, LUA_G_CURSOR, cursor);

			setField(L, -1, LUA_G_GEOMETRY, Geometry::creator());

			IrrlichtComponentSystem* irrSystem = 
				dynamic_cast<IrrlichtComponentSystem*> (scene->requireSystem("Render"));
			scene::ISceneCollisionManager* collMan = irrSystem->getSceneManager()->getSceneCollisionManager();
			setField(L, -1, LUA_G_COLLISION, collMan);
		}
	}

	LuaComponentSystem::~LuaComponentSystem()
	{
		delete mImpl;
	}

	ngn::InheritancePath LuaComponentSystem::hierarchy( const std::string& compName )
	{
		StackMemo memo(mImpl->L);
		if (!mImpl->hierarchy.containsType(compName) && !loadComponent(compName))
		{
			cout << "No Such Component: \"" << compName << "\"" << ":\n";
			LuaUtil::outputErrorMessage(mImpl->L);
		}

		return mImpl->hierarchy.getInheritanceChain(compName);
	}

	Component* LuaComponentSystem::create( const std::string& compName, 
		GameObject& host, const Properties& param )
	{
		lua_State* L = mImpl->L;
		const char* name = compName.c_str();
		StackMemo memo(L);

		if (!loadComponent(compName))
		{
			cout << "No Such Component: \"" << compName << "\"" << ":\n";
			LuaUtil::outputErrorMessage(mImpl->L);
			return nullptr;
		}

		// new object
		lua_getfield(L, -1, LUA_COMPONENT_NEW);
		if (!lua_isfunction(L, -1))
		{
			cout << "field new should be a function" << endl;
			return NULL;
		}

		param.pushSelf(L);
		push<GameObject*>(L, &host);
		if (lua_pcall(L, 2, 1, 0))		// no argument, one result, no errfunc
		{
			LuaUtil::outputErrorMessage(L);
			return NULL;
		}
		
		if (!lua_istable(L, -1))
		{
			cout << "ret value should be table" << endl;
			return NULL;
		}

		luaT::LuaRef object = luaT::LuaRef::fromIndex(L, -1);
		return new LuaComponent(compName, host, object);
	}

	bool LuaComponentSystem::loadComponent( const std::string& compName )
	{
		if (LuaUtil::require(mImpl->L, compName.c_str()) != LuaUtil::REQUIRE_OK)
			return false;
		else if (mImpl->hierarchy.containsType(compName)) 
			return true;
		else
			return initComponent(compName);
	}

	bool LuaComponentSystem::initComponent( const std::string& compName )
	{
		lua_State* L = mImpl->L;

		if (!lua_istable(L, -1))
		{
			lua_pop(L, 1);
			lua_pushstring(L, "table is required for package.loaded's value type");
			return false;
		}

		std::string baseClass;
		if (!loadBase(baseClass))
		{
			LuaUtil::outputErrorMessage(L, "Loading base");
			lua_remove(L, -2);	// remove got table as fail
			return false;
		}

		// set metatable if necessary
		if (!lua_getmetatable(L, -1))
			luaT::Metatable::bind<LuaComponent>(L, -1);
		else
			lua_pop(L, 1);

		// initialize system
		if (!LuaUtil::rawCall(L, -1, LUA_SYSTEM_INIT))
		{
			lua_remove(L, -2);	// remove got table as it init failed
			return false;
		}

		mImpl->hierarchy.registerType(compName, baseClass);
		return true;
	}

	/// @return false if cannot load base and then push error message
	bool LuaComponentSystem::loadBase(std::string& outBase)
	{
		lua_State* L = mImpl->L;
		StackMemo memo(L);

		// static get component base
		lua_pushstring(L, LUA_COMPONENT_BASE);
		lua_rawget(L, -2);
		if (lua_isstring(L, -1))
		{
			outBase = lua_tostring(L, -1);
			memo.restore();	// pop COMPONENT_BASE

			if (!loadComponent(outBase))
			{
				// keep error message and return
				memo.backup();
				return false;
			}
		}

		return true;
	}

	lua_State* LuaComponentSystem::getLuaState()
	{
		return mImpl->L;
	}

	static void invokeSystem(LuaComponentSystem* sys, InheritanceTree& hierarchy, const char* method)
	{
		lua_State* L = sys->getLuaState();

		for (auto& clazz : hierarchy)
		{
			StackMemo memo(L);	// stack recoverer

			XiAssert::isTrue(sys->loadComponent(clazz.typeName));
			if (!LuaUtil::rawCall(L, -1, method))
			{
				string msg = method;
				msg += " Fail";
				LuaUtil::outputErrorMessage(L, msg.c_str());
			}
		}
	}

	void LuaComponentSystem::onStart()
	{
		invokeSystem(this, mImpl->hierarchy, LUA_SYSTEM_START);
	}

	void LuaComponentSystem::onUpdate( const Timeline& time )
	{
		invokeSystem(this, mImpl->hierarchy, LUA_SYSTEM_UPDATE);

		lua_State* L = mImpl->L;
		lua_getglobal(L, LUA_G_TIME);
		setField(L, -1, LUA_G_TIME_GLOBAL, time.getElapsedSeconds());
		setField(L, -1, LUA_G_TIME_CHANGE, time.getLastTimeChange());
		setField(L, -1, LUA_G_TIME_PAUSED, time.isPaused());
		lua_pop(L, 1);
	}

	void LuaComponentSystem::onStop()
	{
		invokeSystem(this, mImpl->hierarchy, LUA_SYSTEM_STOP);
	}

}}
