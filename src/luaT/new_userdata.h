#pragma once
#include "type_traits.h"
#include <cassert>
#include "metatable.h"
#include "reserved.h"

namespace luaT {

	struct UserdataAllocator
	{
		static void* allocate(lua_State* L, size_t size, const char* mtName);
		static void allocate_for_ptr(lua_State* L, void* ptr, const char* mtName);
	};

	/************************************************************************/
	/* 应该保证类的赋值操作中没有副作用。
	/************************************************************************/
	template <typename T, bool isRef, bool isPointer>
	struct NewUserdata_
	{
		// value.
		static void exec(lua_State* L, typename TypeTraits<T>::ParameterType ptr)
		{
			void* mem = UserdataAllocator::allocate(L, sizeof(T), MetatableData<T>::name);
			new (mem) T(ptr);	// copy constructor.
		}
	};

	template <typename T>
	struct NewUserdata_<T, false, true>
	{
		typedef MetatableData<typename TypeTraits<T>::PointeeType> MD;

		// pointer.
		static void exec(lua_State* L, typename TypeTraits<T>::ParameterType ptr)
		{
			UserdataAllocator::allocate_for_ptr(L, ptr, MD::name);
		}
	};

	template <typename T>
	struct NewUserdata_<T, true, false>
	{
		// reference.
		static void exec(lua_State* L, typename TypeTraits<T>::ParameterType ptr)
		{
			NewUserdata_<typename TypeTraits<T>::PointeeType*, false, true>::exec(L, &ptr);
		}
	};

	// new userdata, and use value to initialize it.
	// set its metatable automaticly.
	// @result, userdata is on the top of stack.
	template <typename T>
	inline void newUserdata(lua_State* L, typename TypeTraits<T>::ParameterType value)
	{
		NewUserdata_<T, TypeTraits<T>::isReference, TypeTraits<T>::isPointer>::exec(L, value);
	}

}