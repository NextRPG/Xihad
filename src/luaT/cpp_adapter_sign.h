#pragma once
#include <new>
#include "Lua/lua.hpp"
#include "metatable.h"

#define LUAT_GET_PARAM_ARG_0(base) 
#define LUAT_GET_PARAM_ARG_1(base) LUAT_GET_PARAM_ARG_0(base) A1 arg1 = checkarg<A1>(L, 1+base);
#define LUAT_GET_PARAM_ARG_2(base) LUAT_GET_PARAM_ARG_1(base) A2 arg2 = checkarg<A2>(L, 2+base);
#define LUAT_GET_PARAM_ARG_3(base) LUAT_GET_PARAM_ARG_2(base) A3 arg3 = checkarg<A3>(L, 3+base);
#define LUAT_GET_PARAM_ARG_4(base) LUAT_GET_PARAM_ARG_3(base) A4 arg4 = checkarg<A4>(L, 4+base);
#define LUAT_GET_PARAM_ARG_5(base) LUAT_GET_PARAM_ARG_4(base) A5 arg5 = checkarg<A5>(L, 5+base);
#define LUAT_GET_PARAM_ARG_6(base) LUAT_GET_PARAM_ARG_5(base) A6 arg6 = checkarg<A6>(L, 6+base);

#define LUAT_ALLOCATE_USERDATA void* raw = lua_newuserdata(L, sizeof(U));

#define LUAT_NEW_USERDATA_0 new (raw) U();
#define LUAT_NEW_USERDATA_1 new (raw) U(arg1);
#define LUAT_NEW_USERDATA_2 new (raw) U(arg1, arg2);
#define LUAT_NEW_USERDATA_3 new (raw) U(arg1, arg2, arg3);
#define LUAT_NEW_USERDATA_4 new (raw) U(arg1, arg2, arg3, arg4);
#define LUAT_NEW_USERDATA_5 new (raw) U(arg1, arg2, arg3, arg4, arg5);
#define LUAT_NEW_USERDATA_6 new (raw) U(arg1, arg2, arg3, arg4, arg5, arg6);

#define LUAT_NEW_ON_CPP_0 U* raw = new U();
#define LUAT_NEW_ON_CPP_1 U* raw = new U(arg1);
#define LUAT_NEW_ON_CPP_2 U* raw = new U(arg1, arg2);
#define LUAT_NEW_ON_CPP_3 U* raw = new U(arg1, arg2, arg3);
#define LUAT_NEW_ON_CPP_4 U* raw = new U(arg1, arg2, arg3, arg4);
#define LUAT_NEW_ON_CPP_5 U* raw = new U(arg1, arg2, arg3, arg4, arg5);
#define LUAT_NEW_ON_CPP_6 U* raw = new U(arg1, arg2, arg3, arg4, arg5, arg6);

#define LUAT_SET_METATABLE Metatable::forType<U>(L); lua_setmetatable(L, -2); 

#define LUAT_GET_OBJECT_PTR  O* obj = PointerValue::ptrValue(checkarg<FindPtr<O, ptrcount>::result*>(L, 1));

#define LUAT_CALL_MFUNCTION_0 return (obj->*F) ();
#define LUAT_CALL_MFUNCTION_1 return (obj->*F) (arg1);
#define LUAT_CALL_MFUNCTION_2 return (obj->*F) (arg1, arg2);
#define LUAT_CALL_MFUNCTION_3 return (obj->*F) (arg1, arg2, arg3);
#define LUAT_CALL_MFUNCTION_4 return (obj->*F) (arg1, arg2, arg3, arg4);
#define LUAT_CALL_MFUNCTION_5 return (obj->*F) (arg1, arg2, arg3, arg4, arg5);
#define LUAT_CALL_MFUNCTION_6 return (obj->*F) (arg1, arg2, arg3, arg4, arg5, arg6);

#define LUAT_CALL_CFUNCTION_0 return (*F) ();
#define LUAT_CALL_CFUNCTION_1 return (*F) (arg1);
#define LUAT_CALL_CFUNCTION_2 return (*F) (arg1, arg2);
#define LUAT_CALL_CFUNCTION_3 return (*F) (arg1, arg2, arg3);
#define LUAT_CALL_CFUNCTION_4 return (*F) (arg1, arg2, arg3, arg4);
#define LUAT_CALL_CFUNCTION_5 return (*F) (arg1, arg2, arg3, arg4, arg5);
#define LUAT_CALL_CFUNCTION_6 return (*F) (arg1, arg2, arg3, arg4, arg5, arg6);

#define LUAT_CALL_MFUNCTION_ARG_RET(a, r)	\
			luaL_checkany(L, a+1);			\
			LUAT_GET_OBJECT_PTR				\
			LUAT_GET_PARAM_ARG_##a(1)		\
			LUAT_CALL_MFUNCTION_##a			

#define LUAT_CALL_CFUNCTION_ARG_RET(a, r)	\
			luaL_checkany(L, a);			\
			LUAT_GET_PARAM_ARG_##a(0)		\
			LUAT_CALL_CFUNCTION_##a			

#define LUAT_CONSTRUCT_ARG(n)				\
			static_assert(					\
				!TypeTraits<U>::isPointer,	\
				"U can't be pointer");		\
			luaL_checkany(L, n);			\
			LUAT_GET_PARAM_ARG_##n(0)		\
			LUAT_ALLOCATE_USERDATA			\
			LUAT_NEW_USERDATA_##n			\
			LUAT_SET_METATABLE				\
			return 1;

#define LUAT_NEW_ON_CPP_ARG(n)				\
			static_assert(					\
				!TypeTraits<U>::isPointer,	\
				"U can't be pointer");		\
			luaL_checkany(L, n);			\
			LUAT_GET_PARAM_ARG_##n(0)		\
			LUAT_NEW_ON_CPP_##n				\
			newUserdata<U*>(L, raw);		\
			return 1;

namespace luaT {
	template <typename R>
	struct CFunctionSigAdapter { };

	template <typename R>
	struct CFunctionSigAdapter<R ()> 
	{ static inline R result(lua_State* L, R (* F) ()) { LUAT_CALL_CFUNCTION_ARG_RET(0, 1) } };

	template <typename R, typename A1>
	struct CFunctionSigAdapter<R (A1)> 
	{ static inline R result(lua_State* L, R (* F) (A1)) { LUAT_CALL_CFUNCTION_ARG_RET(1, 1) } };

	template <typename R, typename A1, typename A2>
	struct CFunctionSigAdapter<R (A1, A2)> 
	{ static inline R result(lua_State* L, R (* F) (A1, A2)) { LUAT_CALL_CFUNCTION_ARG_RET(2, 1) } };

	template <typename R, typename A1, typename A2, typename A3>
	struct CFunctionSigAdapter<R (A1, A2, A3)> 
	{ static inline R result(lua_State* L, R (* F) (A1, A2, A3)) { LUAT_CALL_CFUNCTION_ARG_RET(3, 1) } };

	template <typename R, typename A1, typename A2, typename A3, typename A4>
	struct CFunctionSigAdapter<R (A1, A2, A3, A4)> 
	{ static inline R result(lua_State* L, R (* F) (A1, A2, A3, A4)) { LUAT_CALL_CFUNCTION_ARG_RET(4, 1) } };

	template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
	struct CFunctionSigAdapter<R (A1, A2, A3, A4, A5)> 
	{ static inline R result(lua_State* L, R (* F) (A1, A2, A3, A4, A5)) { LUAT_CALL_CFUNCTION_ARG_RET(5, 1) } };

	template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	struct CFunctionSigAdapter<R (A1, A2, A3, A4, A5, A6)> 
	{ static inline R result(lua_State* L, R (* F) (A1, A2, A3, A4, A5, A6)) { LUAT_CALL_CFUNCTION_ARG_RET(6, 1) } };

	//////////////////////////////////////////////////////////////////////////

	template <typename Signature, int ptrcount=0>
	struct MemberFuncSigAdapter { };

	template <typename O, typename R, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(), ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)()) { LUAT_CALL_MFUNCTION_ARG_RET(0, 1) } };

	template <typename O, typename R, typename A1, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1), ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1)) { LUAT_CALL_MFUNCTION_ARG_RET(1, 1) } };

	template <typename O, typename R, typename A1, typename A2, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1, A2), ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1, A2)) { LUAT_CALL_MFUNCTION_ARG_RET(2, 1) } };

	template <typename O, typename R, typename A1, typename A2, typename A3, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1, A2, A3), ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1, A2, A3)) { LUAT_CALL_MFUNCTION_ARG_RET(3, 1) } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4), ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1, A2, A3, A4)) { LUAT_CALL_MFUNCTION_ARG_RET(4, 1) } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4, A5), ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1, A2, A3, A4, A5)) { LUAT_CALL_MFUNCTION_ARG_RET(5, 1) } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4, A5, A6), ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1, A2, A3, A4, A5, A6)) { LUAT_CALL_MFUNCTION_ARG_RET(6, 1) } };

	//////////////////////////////////////////////////////////////////////////
	// const version of memeber functions.
	//////////////////////////////////////////////////////////////////////////

	template <typename O, typename R, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)() const, ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)() const) { LUAT_CALL_MFUNCTION_ARG_RET(0, 1) } };

	template <typename O, typename R, typename A1, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1) const, ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1) const) { LUAT_CALL_MFUNCTION_ARG_RET(1, 1) } };

	template <typename O, typename R, typename A1, typename A2, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1, A2) const, ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1, A2) const) { LUAT_CALL_MFUNCTION_ARG_RET(2, 1) } };

	template <typename O, typename R, typename A1, typename A2, typename A3, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1, A2, A3) const, ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1, A2, A3) const) { LUAT_CALL_MFUNCTION_ARG_RET(3, 1) } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4) const, ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1, A2, A3, A4) const) { LUAT_CALL_MFUNCTION_ARG_RET(4, 1) } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4, A5) const, ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1, A2, A3, A4, A5) const) { LUAT_CALL_MFUNCTION_ARG_RET(5, 1) } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, int ptrcount>
	struct MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4, A5, A6) const, ptrcount> 
	{ static inline R result(lua_State* L, R (O::*F)(A1, A2, A3, A4, A5, A6) const) { LUAT_CALL_MFUNCTION_ARG_RET(6, 1) } };
}
