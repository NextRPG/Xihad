#pragma once
#include "cpp_adapter_sign.h"
#include "type_traits.h"

namespace luaT
{
	template <typename Host, typename FieldType, FieldType Host::* FieldPtr>
	void writeAttrGetterAdapter(Host* object, const FieldType& value)
	{
		object->*FieldPtr = value;
	}

	template <typename Host, typename FieldType, FieldType Host::* FieldPtr>
	FieldType readAttrGetterAdapter(Host* object)
	{
		return object->*FieldPtr;
	}

	//////////////////////////////////////////////////////////////////////////
	// TEMPLATE DESTRUCTOR FOR LUA
	//////////////////////////////////////////////////////////////////////////
	template <class T>
	struct DefaultDestructor
	{
		inline static lua_CFunction result()
		{
			return DestructorAdapter<T>;
		}
	};

	template <typename T>
	struct DefaultDestructor<T*>
	{
		inline static lua_CFunction result()
		{
			return NULL;
		}
	};

	template <typename T>
	struct DefaultDestructor<T&>
	{
		inline static lua_CFunction result()
		{
			return NULL;
		}
	};

	template<typename T>
	int DestructorAdapter(lua_State* L)
	{
		T* ud = checkarg<T*>(L, 1);
		ud->~T();
		return 0;
	}

	template <typename S, S F>
	struct ReturnNothing
	{
	};

	template <typename R, R (* F)(lua_State*)>
	struct ReturnNothing<R (*)(lua_State*), F>
	{
		static int result(lua_State* L)
		{
			F(L);
			return 0;
		}
	};

	template <typename S, S F>
	struct ReturnAsPossible
	{
	};

	template <typename R, R (* F)(lua_State*)>
	struct ReturnAsPossible<R (*)(lua_State*), F>
	{
		static int result(lua_State* L)
		{
			R ret = F(L);
			push<R>(L, ret);
			return 1;
		}
	};

	template <void (* F) (lua_State*)>
	struct ReturnAsPossible<void (*)(lua_State*), F>
	{
		static int result(lua_State* L)
		{
			F(L);
			return 0;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// TEMPLATE C FUNCTION FOR LUA
	// 不提供任何默认值，被调用时会检查参数个数是否一致。
	//////////////////////////////////////////////////////////////////////////
	/************************************************************************/
	/* If you have need to implement more args, do it yourself.
	* Because the default implementatioin is not given, so you can 
	* list your implementation in your own .h or .cpp files.
	* Do not have multi copy of one specification.
	/************************************************************************/
	template <typename S, S* F> 
	struct CFunctionAdapter
	{ };

	template <typename R, typename R (* F) ()>
	struct CFunctionAdapter<R (), F> 
	{ static R result(lua_State* L) { return CFunctionSigAdapter<R ()>::result(L, F);} };

	template <typename R, typename A1, R (* F) (A1)>
	struct CFunctionAdapter<R (A1), F> 
	{ static R result(lua_State* L) { return CFunctionSigAdapter<R (A1)>::result(L, F);} };

	template <typename R, typename A1, typename A2, R (* F) (A1, A2)>
	struct CFunctionAdapter<R (A1, A2), F> 
	{ static R result(lua_State* L) { return CFunctionSigAdapter<R (A1, A2)>::result(L, F);} };

	template <typename R, typename A1, typename A2, typename A3, R (* F) (A1, A2, A3)>
	struct CFunctionAdapter<R (A1, A2, A3), F> 
	{ static R result(lua_State* L) { return CFunctionSigAdapter<R (A1, A2, A3)>::result(L, F);} };

	template <typename R, typename A1, typename A2, typename A3, typename A4, R (* F) (A1, A2, A3, A4)>
	struct CFunctionAdapter<R (A1, A2, A3, A4), F> 
	{ static R result(lua_State* L) { return CFunctionSigAdapter<R (A1, A2, A3, A4)>::result(L, F);} };

	template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, R (* F) (A1, A2, A3, A4, A5)>
	struct CFunctionAdapter<R (A1, A2, A3, A4, A5), F> 
	{ static R result(lua_State* L) { return CFunctionSigAdapter<R (A1, A2, A3, A4, A5)>::result(L, F);} };

	template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, R (* F) (A1, A2, A3, A4, A5, A6)>
	struct CFunctionAdapter<R (A1, A2, A3, A4, A5, A6), F> 
	{ static R result(lua_State* L) { return CFunctionSigAdapter<R (A1, A2, A3, A4, A5, A6)>::result(L, F);} };

	//////////////////////////////////////////////////////////////////////////
	// TEMPLATE CONSTRUCTOR FOR LUA
	// 不提供任何默认值，被调用时会检查参数个数是否一致。
	//////////////////////////////////////////////////////////////////////////
	template <typename Signature>
	struct ConstructorAdapter 
	{ static int result(lua_State* L); };

	template <typename U>
	struct ConstructorAdapter<U (void)> 
	{ static int result(lua_State* L) { LUAT_CONSTRUCT_ARG(0) } };

	template <typename U, typename A1>
	struct ConstructorAdapter<U (A1)> 
	{ static int result(lua_State* L) { LUAT_CONSTRUCT_ARG(1) } };

	template <typename U, typename A1, typename A2>
	struct ConstructorAdapter<U (A1, A2)> 
	{ static int result(lua_State* L) { LUAT_CONSTRUCT_ARG(2) } };

	template <typename U, typename A1, typename A2, typename A3>
	struct ConstructorAdapter<U (A1, A2, A3)> 
	{ static int result(lua_State* L) { LUAT_CONSTRUCT_ARG(3) } };

	template <typename U, typename A1, typename A2, typename A3, typename A4>
	struct ConstructorAdapter<U (A1, A2, A3, A4)> 
	{ static int result(lua_State* L) { LUAT_CONSTRUCT_ARG(4) } };

	template <typename U, typename A1, typename A2, typename A3, typename A4, typename A5>
	struct ConstructorAdapter<U (A1, A2, A3, A4, A5)> 
	{ static int result(lua_State* L) { LUAT_CONSTRUCT_ARG(5) } };

	template <typename U, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	struct ConstructorAdapter<U (A1, A2, A3, A4, A5, A6)> 
	{ static int result(lua_State* L) { LUAT_CONSTRUCT_ARG(6) } };

	template <typename Signature>
	struct NewOnCppAdapter 
	{ static int result(lua_State* L); };

	template <typename U>
	struct NewOnCppAdapter<U (void)> 
	{ static int result(lua_State* L) { LUAT_NEW_ON_CPP_ARG(0) } };

	template <typename U, typename A1>
	struct NewOnCppAdapter<U (A1)> 
	{ static int result(lua_State* L) { LUAT_NEW_ON_CPP_ARG(1) } };

	template <typename U, typename A1, typename A2>
	struct NewOnCppAdapter<U (A1, A2)> 
	{ static int result(lua_State* L) { LUAT_NEW_ON_CPP_ARG(2) } };

	template <typename U, typename A1, typename A2, typename A3>
	struct NewOnCppAdapter<U (A1, A2, A3)> 
	{ static int result(lua_State* L) { LUAT_NEW_ON_CPP_ARG(3) } };

	template <typename U, typename A1, typename A2, typename A3, typename A4>
	struct NewOnCppAdapter<U (A1, A2, A3, A4)> 
	{ static int result(lua_State* L) { LUAT_NEW_ON_CPP_ARG(4) } };

	template <typename U, typename A1, typename A2, typename A3, typename A4, typename A5>
	struct NewOnCppAdapter<U (A1, A2, A3, A4, A5)> 
	{ static int result(lua_State* L) { LUAT_NEW_ON_CPP_ARG(5) } };

	template <typename U, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	struct NewOnCppAdapter<U (A1, A2, A3, A4, A5, A6)> 
	{ static int result(lua_State* L) { LUAT_NEW_ON_CPP_ARG(6) } };

	//////////////////////////////////////////////////////////////////////////
	// TEMPLATE MEMBER FUNCTION FOR LUA
	// 此实现不提供任何默认值，被调用时会检查参数个数是否一致。
	//////////////////////////////////////////////////////////////////////////

	template <typename Signature, Signature MemberFunc, int ptrcount=0>
	struct MemberFuncAdapter { };

	template <typename O, typename R, R (O::* F)(), int ptrcount>
	struct MemberFuncAdapter<R (O::*)(), F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(), ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, R (O::* F)(A1), int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1), F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1), ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, typename A2, R (O::* F)(A1, A2), int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1, A2), F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1, A2), ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, typename A2, typename A3, R (O::* F)(A1, A2, A3), int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1, A2, A3), F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1, A2, A3), ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, R (O::* F)(A1, A2, A3, A4), int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1, A2, A3, A4), F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4), ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, R (O::* F)(A1, A2, A3, A4, A5), int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1, A2, A3, A4, A5), F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4, A5), ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, R (O::* F)(A1, A2, A3, A4, A5, A6), int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1, A2, A3, A4, A5, A6), F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4, A5, A6), ptrcount>::result(L, F); } };

	//////////////////////////////////////////////////////////////////////////
	// const version of memeber functions.
	//////////////////////////////////////////////////////////////////////////
	template <typename O, typename R, R (O::* F)() const, int ptrcount>
	struct MemberFuncAdapter<R (O::*)() const, F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)() const, ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, R (O::* F)(A1) const, int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1) const, F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1) const, ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, typename A2, R (O::* F)(A1, A2) const, int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1, A2) const, F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1, A2) const, ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, typename A2, typename A3, R (O::* F)(A1, A2, A3) const, int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1, A2, A3) const, F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1, A2, A3) const, ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, R (O::* F)(A1, A2, A3, A4) const, int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1, A2, A3, A4) const, F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4) const, ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, R (O::* F)(A1, A2, A3, A4, A5) const, int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1, A2, A3, A4, A5) const, F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4, A5) const, ptrcount>::result(L, F); } };

	template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, R (O::* F)(A1, A2, A3, A4, A5, A6) const, int ptrcount>
	struct MemberFuncAdapter<R (O::*)(A1, A2, A3, A4, A5, A6) const, F, ptrcount> 
	{ static R result(lua_State* L) { return MemberFuncSigAdapter<R (O::*)(A1, A2, A3, A4, A5, A6) const, ptrcount>::result(L, F); } };

}

