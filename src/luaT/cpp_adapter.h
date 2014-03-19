#pragma once
#include "cpp_adapter_func.h"

#define luaT_static namespace {
#define luaT_endofregs			{ NULL, NULL }, 

#define luaT_defRegsBgn(name) 	::luaL_Reg name[] = {
#define luaT_defRegsEnd				luaT_endofregs						\
								};

//////////////////////////////////////////////////////////////////////////
//! function adapter macros
#define luaT_constructor(Signature) ::luaT::ConstructorAdapter<Signature>::result
#define luaT_newoncpp(Signature)	::luaT::NewOnCppAdapter<Signature>::result
#define luaT_destructor(Type)		::luaT::DefaultDestructor<Type>::result

#define luaT_cfunction(funcImpl)										\
	::luaT::ReturnAsPossible<decltype(&::luaT::CFunctionAdapter<decltype(funcImpl), funcImpl >::result), &::luaT::CFunctionAdapter<decltype(funcImpl), funcImpl >::result>::result

#define luaT_mfunction(funcImpl)										\
	::luaT::ReturnAsPossible<decltype(&::luaT::MemberFuncAdapter<decltype(&funcImpl), &funcImpl >::result), &::luaT::MemberFuncAdapter<decltype(&funcImpl), &funcImpl >::result>::result

#define luaT_mfunction_nret(funcImpl)									\
	::luaT::ReturnNothing<decltype(&::luaT::MemberFuncAdapter<decltype(&funcImpl), &funcImpl >::result), &::luaT::MemberFuncAdapter<decltype(&funcImpl), &funcImpl >::result>::result

#define luaT_read_attribute(Type, attrName)								\
	luaT_cfunction((::luaT::readAttrGetterAdapter<Type, decltype(Type::attrName), &Type::attrName>))

#define luaT_write_attribute(Type, attrName)							\
	luaT_cfunction((::luaT::writeAttrGetterAdapter<Type, decltype(Type::attrName), &Type::attrName>))

#define luaT_cfunction_ovl(Signature, funcImpl)							\
	::luaT::ReturnAsPossible<decltype(&::luaT::CFunctionAdapter<Signature, funcImpl >::result), &::luaT::CFunctionAdapter<Signature, funcImpl >::result>::result

#define luaT_mfunction_ovl(Signature, funcImpl)							\
	::luaT::ReturnAsPossible<decltype(&::luaT::MemberFuncAdapter<Signature, &##funcImpl >::result), &::luaT::MemberFuncAdapter<Signature, &##funcImpl >::result>::result

//////////////////////////////////////////////////////////////////////////
//! Named version
#define luaT_lnamedfunc(funcName)						\
		{ #funcName, funcName }

#define luaT_lnnamefunc(funcName, customName)			\
		{ #customName, funcName }

#define luaT_cnamedfunc(funcName)						\
		{ #funcName, luaT_cfunction(funcName) }
#define luaT_cnnamefunc(funcName, customName)			\
		{ #customName, luaT_cfunction(funcName) }

#define luaT_mnamedfunc(Type, funcName)					\
		{ #funcName, luaT_mfunction(Type::funcName) }
#define luaT_mnnamefunc(Type, funcName, customName)		\
		{ #customName, luaT_mfunction(Type::funcName) }

#define luaT_mnamedfunc_nret(Type, funcName)					\
		{ #funcName, luaT_mfunction_nret(Type::funcName) }
#define luaT_mnnamefunc_nret(Type, funcName, customName)		\
		{ #customName, luaT_mfunction_nret(Type::funcName) }

#define luaT_mnamed_readattr(Type, attrName)					\
		{ "get"#attrName, luaT_read_attribute(Type, attrName) }
#define luaT_mnname_readattr(Type, attrName, customName)		\
		{ #customName, luaT_read_attribute(Type, attrName) }

#define luaT_mnamed_writeattr(Type, attrName)					\
		{ "set"#attrName, luaT_write_attribute(Type, attrName) }
#define luaT_mnname_writeattr(Type, attrName, customName)		\
		{ #customName, luaT_write_attribute(Type, attrName) }

#define luaT_cnamedfunc_ovl(Signature, funcName)		\
		{ #funcName, luaT_cfunction_ovl(Signature, funcName) }

#define luaT_mnamedfunc_ovl(Type, Signature, funcName)	\
		{ #funcName, luaT_mfunction_ovl(Signature, Type::funcName) }

// #define luaT_pfunction(funcImpl)										\
// 		::luaT::MemberFuncAdapter<decltype(&##funcImpl), &##funcImpl, 1>::result
// #define luaT_mfunction_ptr(funcImpl, ptrcount)							\
// 		::luaT::MemberFuncAdapter<decltype(&##funcImpl), &##funcImpl, ptrcount>::result
// #define luaT_pfunction_ovl(signature, funcImpl)							\
// 		::luaT::MemberFuncAdapter<signature, &funcImpl, 1>::result
// #define luaT_mfunction_ptr_ovl(signature, funcImpl, ptrcount) :			\
// 		:luaT::MemberFuncAdapter<signature, &##funcImpl, ptrcount>::result
// #define luaT_pnamedfunc(type, funcName) { #funcName, luaT_pfunction(##type::##funcName) }
// #define luaT_mnamedfunc_ptr(type, funcName, ptrcount) { #funcName, luaT_mfunction_ptr(##type::##funcName, ptrcount) }
// #define luaT_pnamedfunc_ovl(type, signature, funcName) { #funcName, luaT_pfunction_ovl(signature, ##type::##funcImp) }
// #define luaT_mnamedfunc_ptr_ovl(type, signature, funcName, ptrcount) { #funcName, luaT_mfunction_ptr_ovl(signature, ##type::##funcName, ptrcount) }
