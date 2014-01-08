#pragma once
#include "Lua/lua.hpp"
#include "type_traits.h"
#include "same_type.h"
#include "xassert.h"

namespace luaT
{
	struct GeneralMetatableFactory
	{
		static void create(lua_State* L, const char* name, luaL_Reg* regs, lua_CFunction gc);
	};

	/************************************************************************/
	/* Shortcut to register a metatable for some a class T.
	/************************************************************************/
	template <typename T, typename P = NullType>
	class MetatableFactory
	{
	public:
		/************************************************************************/
		/* The product metatable will set index himself,
		 * And the metatable of this product is his Parent class's metatable.
		 * If ParentType isnot given, NullType is the default value, and in this
		 * situation, no metatable for the product.
		 * 
		 * The product won't exist on the stack after this method, you can call:
		 * "getMetatable<T>(); " to get it.
		/************************************************************************/
		static inline void create(lua_State* L, luaL_Reg* regs, lua_CFunction gc = DefaultDestructor<T>::result())
		{
			// create metatable and init it with parent
			GeneralMetatableFactory::create(L, MetatableData<T>::name, regs, gc);
			ParentMetatalbeSetter<P, T>::exec(L, MetatableData<T>::name);
			lua_pop(L, 1);
		}

		static inline void createNull(lua_State* L, lua_CFunction gc = DefaultDestructor<T>::result())
		{
			luaL_Reg nullReg[] = { luaT_endofregs };
			create(L, nullReg, gc);
		}
	};

	struct InheritenceResolver
	{
		static void link(lua_State* L, int offset, const char* mtName);
	};

	template <typename P, typename T>
	struct ParentMetatalbeSetter
	{
		typedef typename TypeTraits<T>::BaseType* _1stPtr2TBase;
		typedef typename TypeTraits<P>::BaseType* _1stPtr2PBase;

		static inline void exec(lua_State* L, const char* currentClassName)
		{
			xassert(MetatableData<P>::polymorphic && "MetatableData of P must allow poly");

			// THIS SHOULD BE CONSTANT!!!
			int PTR_OFFSET = ((int)(_1stPtr2PBase)(_1stPtr2TBase) 1) - 1;

			if (PTR_OFFSET != 0)
				xassert(TypeTraits<P>::ptrcount == 0 && "Cannot cast from T** to P**");

			InheritenceResolver::link(L, PTR_OFFSET, MetatableData<P>::name);
		}
	};

	template <typename T>
	struct ParentMetatalbeSetter<NullType, T>
	{
		static inline void exec(lua_State* L, const char* currentClassName)
		{ // Do nothing.
		}
	};
}
