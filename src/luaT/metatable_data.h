#pragma once
#include <typeinfo>
#include "type_traits.h"
#include "config.h"

#ifdef LUAT_USE_METADATA
#define luaT_defMetaData(Type, Poly)								\
namespace luaT														\
{																	\
	const char* const												\
	::luaT::MetatableData<											\
	typename ::luaT::TypeTraits<Type>::NoConstType>					\
	::uniqueTypeName =												\
		typeid(::luaT::TypeTraits<Type>::NoConstType).name();		\
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
		inline static const char* const name()
		{
			return uniqueTypeName;
		}

		inline static bool isPolymorphic()
		{
			return polymorphic;
		}

		static const char* const uniqueTypeName;
		static const bool polymorphic;
	};
}

#else

#define luaT_defMetaData(Type, Poly)
namespace luaT
{
	template <typename T> 
	struct MetatableData
	{
		inline static const char* const name()
		{
			return typeid(TypeTraits<T>::NoConstType).name();
		}

		inline static bool isPolymorphic()
		{
			return true;
		}
	};
}

#endif
