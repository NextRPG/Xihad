#pragma once
#include "param_types.h"
#include "Lua/lua.hpp"
#include "userdata_checker.h"
#include "xassert.h"

#define luaT_variable(L, idx, Type, var_name) \
	Type var_name = ::luaT::checkarg<Type>(L, idx)

namespace luaT
{
	//////////////////////////////////////////////////////////////////////////
	// Generic lua type judgement.
	// 当不是从Lua调用C时使用，这样不会使程序ABORT。
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline bool is(lua_State* L, int idx)
	{
		typedef typename TypeTraits<T>::NoConstType NonConstT;
		return is_<NonConstT>(L, idx);
	}

	template <typename T>
	inline bool is_(lua_State* L, int idx)
	{
		static_assert(sizeof(T), "Give me T.h if T is not a pointer");
		static_assert(TypeTraits<T>::isPointer, "T must be a pointer, ie userdata in Lua");
		typedef UserdataTypeChecker<T, TypeTraits<T>::isPointer> TChcker;

		bool valid;
		void* ud = UserdataTypeCheckerNT::castUserdata(L, idx, &valid);

		if (valid)
		{
			TChcker::shiftUserdataOnChecking(L, idx, &ud);
			return ud != NULL;
		}
		else
		{
			return false;
		}
	}

	template <>
	inline bool is_<int>(lua_State* L, int idx)
	{
		return lua_isnumber(L, idx) == 1;
	}

	template <>
	inline bool is_<double>(lua_State* L, int idx)
	{
		return lua_isnumber(L, idx) == 1;
	}

	template <>
	inline bool is_<float>(lua_State* L, int idx)
	{
		return lua_isnumber(L, idx) == 1;
	}

	template <>
	inline bool is_<const char*>(lua_State* L, int idx)
	{
		return lua_isstring(L, idx) == 1;
	}

	template <>
	inline bool is_<LString>(lua_State* L, int idx)
	{
		return lua_isstring(L, idx) == 1;
	}

	template <>
	inline bool is_<bool>(lua_State* L, int idx)
	{
		return lua_isboolean(L, idx) == 1;
	}

	template <>
	inline bool is_<lua_State*>(lua_State* L, int idx)
	{
		return lua_isthread(L, idx) == 1;
	}

	template <>
	inline bool is_<Thread>(lua_State* L, int idx)
	{
		return lua_isthread(L, idx) == 1;
	}

	template <>
	inline bool is_<lua_CFunction>(lua_State* L, int idx)
	{
		return lua_iscfunction(L, idx) == 1;
	}

	template <>
	inline bool is_<Nil>(lua_State* L, int idx)
	{
		return lua_isnil(L, idx) == 1;
	}

	//////////////////////////////////////////////////////////////////////////
	// Generic to function.
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline T to(lua_State* L, int idx)
	{
		typedef typename TypeTraits<T>::NoConstType NoConstT;
		return to_<NoConstT>(L, idx);
	}

	template <typename T>
	inline T to_(lua_State* L, int idx)
	{
		static_assert(TypeTraits<T>::isPointer, "T must be a pointer, ie userdata in Lua");
		typedef UserdataTypeChecker<T, TypeTraits<T>::isPointer> TChecker;
		void* ud = UserdataTypeCheckerNT::castUserdata(L, idx);
		TChecker::shiftUserdataOnChecking(L, idx, &ud);
		return (T) ud;
	}

	template <>
	inline int to_<int>(lua_State* L, int idx)
	{
		return lua_tointeger(L, idx);
	}

	template <>
	inline double to_<double>(lua_State* L, int idx)
	{
		return lua_tonumber(L, idx);
	}

	template <>
	inline float to_<float>(lua_State* L, int idx)
	{
		return (float) lua_tonumber(L, idx);
	}

	template <>
	inline char* to_<char*>(lua_State* L, int idx)
	{
		return const_cast<char*>(lua_tostring(L, idx));
	}

	template <>
	inline LString to_<LString>(lua_State* L, int idx)
	{
		size_t sz;
		const char* s = lua_tolstring(L, idx, &sz);
		return LString(s, sz);
	}

	template <>
	inline bool to_<bool>(lua_State* L, int idx)
	{
		return lua_toboolean(L, idx) == 1;
	}

	template <>
	inline lua_State* to_<lua_State*>(lua_State* L, int idx)
	{
		return lua_tothread(L, idx);
	}

	template <>
	inline lua_CFunction to_<lua_CFunction>(lua_State* L, int idx)
	{
		return lua_tocfunction(L, idx);
	}

	template <>
	inline Nil to_<Nil>(lua_State* L, int idx)
	{
		return Nil();
	}

	//////////////////////////////////////////////////////////////////////////
	// C TypeChecker
	// 当不是从 Lua 调用 C 时可以使用。
	//////////////////////////////////////////////////////////////////////////

	template <typename V>
	inline V c_typecheck(lua_State* L, int idx)
	{
		if (is<V>(L, idx))
		{
			return to<V>(L, idx);
		}
		else 
		{
			xassert(false && "value is not the right type");

			// 因为是模板函数，不能返回一个通用的值，所以抛出异常。代码并不会执行到此处
			// 因为在之前必定断言失败，这里只是为了防止编译器强制要求返回值（很明显我不知道该返回什么）。
			throw 0;	
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// TEMPLATE ARGUMENT CHECK FOR LUA->C
	// 只有当从 Lua 调用 C 时使用，否则会直接调用abort，不利于调试。
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline T optarg(lua_State* L, int idx, typename TypeTraits<T>::ParameterType defaultValue)
	{
		if (lua_isnone(L, idx) || lua_isnil(L, idx))
		{
			return defaultValue;
		}
		else
		{
			return checkarg<T>(L, idx);
		}
	}
	
	/// 从栈中指定位置返回元素
	/**
	 * - 在 check 过程中，一切 const 修饰都会变得无效。即： checkarg<const T> == checkarg<T>。
	 * - 指针类型不匹配的话，会返回空指针
	 * - 非指针类型不匹配的话，会造成 lua type error
	 * - 不建议 check object，那样会出现多次对象拷贝
	 * - check reference 和 check pointer 的效率是一样的
	 * - 支持多态
	 */
	template <typename T>
	inline T checkarg(lua_State* L, int idx)
	{
		typedef typename TypeTraits<T>::NoConstType NonConstT;
		return checkarg_<NonConstT>(L, idx);
	}

	template <typename T>
	inline T checkarg_(lua_State* L, int idx)
	{
		static_assert(sizeof(T), "Give me T.h if T is not a pointer");
		return UserdataTypeChecker<T, TypeTraits<T>::isPointer>::get(L, idx);
	}

	template <>
	inline int checkarg_<int>(lua_State* L, int idx)
	{
		return luaL_checkint(L, idx);
	}

	template <>
	inline double checkarg_<double>(lua_State* L, int idx)
	{
		return luaL_checknumber(L, idx);
	}

	template <>
	inline float checkarg_<float>(lua_State* L, int idx)
	{
		return (float) luaL_checknumber(L, idx);
	}

	template <>
	inline char* checkarg_<char*>(lua_State* L, int idx)
	{
		return const_cast<char*> (luaL_checkstring(L, idx));
	}

	template <>
	inline LString checkarg_<LString>(lua_State* L, int idx)
	{
		size_t sz;
		const char* ch = luaL_checklstring(L, idx, &sz);
		return LString(ch, sz);
	}

	template <>
	inline bool checkarg_<bool>(lua_State* L, int idx)
	{
		if (!lua_isboolean(L, idx))
			luaL_typerror(L, idx, "boolean");

		return lua_toboolean(L, idx) ? true : false;
	}

	template <>
	inline lua_State* checkarg_<lua_State*>(lua_State* L, int idx)
	{
		if (!lua_isthread(L, idx))
			luaL_typerror(L, idx, "Thread");

		return lua_tothread(L, idx);
	}

	template <>
	inline lua_CFunction checkarg_<lua_CFunction>(lua_State* L, int idx)
	{
		if (!lua_iscfunction(L, idx))
			luaL_typerror(L, idx, "lua_CFunction");

		return lua_tocfunction(L, idx);
	}

}    