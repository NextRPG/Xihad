#pragma once
#include <typeinfo>
#include "type_traits.h"

// #define luaT_defClass(Type, polymorphic)							\
// 	template<>														\
// 	struct ::luaT::MetatableData<Type>								\
// 	{																\
// 		static const char* const name;								\
// 		static const bool polymorphic;								\
// 		typedef char POLY[(bool)polymorphic+1];						\
// 	};	
// 
// #define luaT_impClass(Type)											\
// 	const char* const												\
// 	::luaT::MetatableData<											\
// 		typename ::luaT::TypeTraits<Type>::NoConstType>				\
// 	::name = typeid(::luaT::TypeTraits<Type>::NoConstType).name();	

#define luaT_defMetaData(Type, Poly)									\
	namespace luaT														\
	{																	\
		const char* const												\
		::luaT::MetatableData<											\
			typename ::luaT::TypeTraits<Type>::NoConstType>				\
		::name = typeid(::luaT::TypeTraits<Type>::NoConstType).name();	\
																		\
		const bool														\
		::luaT::MetatableData<											\
			typename ::luaT::TypeTraits<Type>::NoConstType>				\
		::polymorphic = (Poly);											\
	}																	

namespace luaT
{
	template <typename U>
	struct MetatableData
	{
		static const char* const name;
		static const bool polymorphic;
	};
}

