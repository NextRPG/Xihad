#include <luaT/luaT.h>
#include <iostream>
#include <ScriptEngine/LuaUtil.h>
#include <irrlicht/dimension2d.h>
#include "ChessboardGraph.h"
#include "RoutingSource.h"
#include "SinglePointSource.h"
#include "RoutingTarget.h"
#include "SinglePointTarget.h"
#include "MaxCostTarget.h"

using namespace std;
using namespace luaT;
using namespace xihad;

typedef RoutingSource<Vec2> RoutingSourceT;
typedef WeightedGraph<Vec2, double> WeightedGraphT;
typedef RoutingTarget<Vec2, double> RoutingTargetT;
typedef SinglePointSource<Vec2> SinglePointSourceT;

class LuaChessboardGraph : public ChessboardGraph
{
public:
	explicit LuaChessboardGraph(unsigned width, unsigned height, LuaRef ref) : 
		ChessboardGraph(irr::core::dimension2du(width, height)), 
		delegateObject(ref) {}

	virtual double getInitCost( Vec2 start ) 
	{
		return 0;
	}

	virtual double computeCost( Vec2 from, Vec2 to ) 
	{
		lua_State* L = delegateObject.getState();
		StackMemo memo(L);
		delegateObject.pushSelf();
		lua_getfield(L, -1, "computeCost");

		if (luaT::pcall(L, 1, 0, delegateObject, from.X, from.Y, to.X, to.Y) != 0) 
		{
			script::LuaUtil::outputErrorMessage(L, "computeCost");
			throw std::exception("script error");
		}

		return luaL_checknumber(L, -1);
	}

	virtual double getStayCost( Vec2 target ) 
	{
		return 0;
	}

private:
	LuaRef delegateObject;
};


luaT_static SinglePointSourceT newSinglePointSource(int x, int y)
{
	return SinglePointSourceT(Vec2(x, y));
}}

static int route(lua_State* L)
{
	class PushToLuaTable
	{
	public:
		PushToLuaTable(LuaRef table) : array(table), idx(1) 
		{
		}

		void push_back(Vec2 point)
		{
			lua_State* L = array.getState();
			StackMemo memo(L);
			array.pushSelf();
			insert(L, point.X);
			insert(L, point.Y);
		}

		void insert(lua_State* L, double n)
		{
			setField(L, -1, idx, n);
			++idx;
		}

	private:
		LuaRef array;
		int idx;
	};

	luaT_variable(L, 1, RoutingSourceT&, source);
	luaT_variable(L, 2, RoutingTargetT&, target);
	luaT_variable(L, 3, WeightedGraphT&, graph);

	AStarRouter<Vec2, double> router(&graph);
	double cost;
	if (lua_gettop(L) == 3)
	{
		cost = router.route(source, target, (PushToLuaTable*)0);
	}
	else
	{
		PushToLuaTable adapter(LuaRef::fromIndex(L, 4));
		cost = router.route(source, target, &adapter);
	}

	lua_pushnumber(L, cost);
	return 1;
}


class MultiPointsSource : public RoutingSourceT
{
public:
	explicit MultiPointsSource(LuaRef table) : array(table) {}

	virtual unsigned getSourceCount() 
	{
		lua_State* L = array.getState();
		StackMemo memo(L);
		array.pushSelf();
		size_t len = lua_objlen(L, -1);
		return len/2;
	}

	virtual Vec2 getSource( unsigned index ) 
	{
		lua_State* L = array.getState();
		StackMemo memo(L);
		array.pushSelf();
		
		int x = getInt(L, 2*index+1);
		int y = getInt(L, 2*index+2);
		return Vec2(x, y);
	}

	int getInt(lua_State* L, int idx)
	{
		lua_rawgeti(L, -1, idx);
		int n = luaL_checkinteger(L, -1);
		lua_pop(L, 1);
		return n;
	}

private:
	LuaRef array;
};

static int traversePoints(lua_State* L)
{
	luaT_variable(L, 1, MaxCostTarget*, target);
	luaL_checktype(L, 2, LUA_TFUNCTION);

	for (Vec2 point : target->all)
	{
		lua_pushvalue(L, 2);
		luaT::call(L, 0, point.X, point.Y);
	}

	return 0;
}

static int luaopen_Astar_impl(lua_State* L)
{
	MetatableFactory<WeightedGraphT>::createNull(L);
	MetatableFactory<LuaChessboardGraph, WeightedGraphT>::createNull(L);

	MetatableFactory<RoutingSourceT>::createNull(L);
	MetatableFactory<SinglePointSourceT, RoutingSourceT>::createNull(L);
	MetatableFactory<MultiPointsSource, RoutingSourceT>::createNull(L);

	MetatableFactory<RoutingTargetT>::createNull(L);
	MetatableFactory<SinglePointTarget, RoutingTargetT>::createNull(L);

	luaT_defRegsBgn(maxCost)
		luaT_lnamedfunc(traversePoints),
	luaT_defRegsEnd
	MetatableFactory<MaxCostTarget, RoutingTargetT>::create(L, maxCost);

	luaT_defRegsBgn(astar)
		luaT_lnnamefunc(luaT_constructor(LuaChessboardGraph(unsigned, unsigned, LuaRef)), newChessboard),
		luaT_lnnamefunc(luaT_constructor(SinglePointTarget(int, int)), newSinglePointTarget),
		luaT_lnnamefunc(luaT_constructor(MaxCostTarget(double)), newMaxCostTarget),
		luaT_lnnamefunc(luaT_constructor(MultiPointsSource(LuaRef)), newMultiPointsSource),
		luaT_cnamedfunc(newSinglePointSource),
		luaT_lnamedfunc(route),
	luaT_defRegsEnd
	luaL_register(L, "Astar", astar);
	return 1;
}

/**
 * local graph = Astar.newChessboard(50, 50, delegate)
 * delegate:setRoutingObject(hero)
 * delegate:computeCost(x1, x2, y1, y1)
 * 
 * local source = Astar.newSinglePointSource(2,3)
 * source = Astar.newMultiPointsSource(t)
 * 
 * local path = {}
 * local target = Astar.newSinglePointTarget(10, 20)
 * Astar.route(source, target, graph, path)
 * 
 * target = Astar.newMaxCostTarget(20)
 * Astar.route(source, target, graph)
 * target:traversePoints(func)
 */
extern "C" __declspec(dllexport) int luaopen_ArtIn(lua_State* L)
{
	return luaopen_Astar_impl(L);
}

