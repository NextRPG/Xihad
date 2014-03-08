#pragma once
#include "Lua/lua.hpp"
#include "stack_ops.h"

namespace luaT
{

	template <typename T, bool isPointer>	// not pointer, value or reference
	struct UserdataTypeChecker
	{
		typedef typename TypeTraits<T>::BaseType BaseType;
		typedef BaseType* PTR;

		inline static BaseType& get(lua_State* L, int idx)
		{
			PTR ptr = UserdataTypeChecker<PTR, true>::get(L, idx);
			if (ptr == nullptr)
 				luaL_typerror(L, idx, 
					MetatableData<typename TypeTraits<T>::PointeeType>::name);

			return *ptr;	// copy if not call by reference.
		}
	};


	struct UserdataTypeCheckerNT
	{
		static void* castUserdata(lua_State* L, int idx);
		static void* shiftUserdata(lua_State* L, void* userdata, int targetMtIndex);
	};

	struct PolyClassChecker
	{
		static void* noShiftChecking(lua_State* L, int idx, void* ud, const char* mtName);
		static void* shiftOnChecking(lua_State* L, int idx, void* ud, const char* mtName);
	};


	template <typename T>
	struct UserdataTypeChecker<T, true>
	{
		// These parameters may be T's derived class,
		// We must calculate the offset from T to Derived class, then cast.
		static void shiftUserdataOnChecking(lua_State* L, int idx, void** ud_ptr)
		{
			typedef typename TypeTraits<T>::PointeeType PType;
			const char* mtName = MetatableData<PType>::name;

			if (MetatableData<PType>::polymorphic)
				*ud_ptr = PolyClassChecker::shiftOnChecking(L, idx, *ud_ptr, mtName);
			else 
				*ud_ptr = PolyClassChecker::noShiftChecking(L, idx, *ud_ptr, mtName);
		}

		static T get(lua_State* L, int idx)
		{
			void* userdata = 0;
			if (userdata = UserdataTypeCheckerNT::castUserdata(L, idx))
				shiftUserdataOnChecking(L, idx, &userdata);	// push target type metatable.

			// null is allowed
			return (T) userdata;
		}
	};

}

