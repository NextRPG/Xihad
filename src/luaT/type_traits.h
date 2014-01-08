#pragma once
#include "null_type.h"
#pragma warning(disable:4180)	// const to function is ignored.

namespace luaT {
	template <class T>
	class TypeTraits
	{
	public:
		template <class U> struct RefTraits
		{
			typedef U DerefType;
			typedef U& AddrefType;
		};

		template <class U> struct RefTraits<U&>
		{
			typedef U DerefType;
			typedef U& AddrefType;
		};

		template <class U> struct PointerTraits
		{
			enum { isPointer = false, isReference = false, ptrcount = 0, };
			typedef NullType PointeeType;
			typedef U BaseType;
		};

		template <class U> struct PointerTraits<U&>
		{
			enum { isPointer = false, isReference = true, ptrcount = PointerTraits<U>::ptrcount, };
			typedef U PointeeType;
			typedef typename PointerTraits<U>::BaseType BaseType;
		};

		template <class U> struct PointerTraits<U*>
		{
			enum { isPointer = true, isReference = false, ptrcount = PointerTraits<U>::ptrcount+1, };
			typedef U PointeeType;
			typedef typename PointerTraits<U>::BaseType BaseType;
		};

		template <typename T> struct ParameterTraits
		{
			typedef T& result;
		};

		template <typename T> struct ParameterTraits<T&>
		{
			typedef T& result;
		};

		template <typename T> struct ParameterTraits<T*>
		{
			typedef T* result;
		};

		template <> struct ParameterTraits<int>		{ typedef int result; };
		template <> struct ParameterTraits<double>	{ typedef double result; };	
		template <> struct ParameterTraits<float>	{ typedef float result; };
		template <> struct ParameterTraits<bool>	{ typedef bool result; };
		template <> struct ParameterTraits<void*>	{ typedef void* result; };
		template <> struct ParameterTraits<char>	{ typedef char result; };
		template <> struct ParameterTraits<short>	{ typedef short result; };
		template <> struct ParameterTraits<long>	{ typedef long result; };
		template <> struct ParameterTraits<unsigned>{ typedef unsigned result; };
		template <> struct ParameterTraits<unsigned char> { typedef unsigned char result; };

		template <typename T>
		struct ConstTraits
		{
			enum { result = false, };
			typedef T DeconstBaseType;
			typedef T NonConstType;
			typedef const T ConstBaseType;
		};

		template <typename T>
		struct ConstTraits<const T>
		{
			enum { result = true, };
			typedef T DeconstBaseType;
			typedef T NonConstType;
			typedef const T ConstBaseType;
		};

		template <typename T>
		struct ConstTraits<T&>
		{
			enum { result = true, };
			typedef typename ConstTraits<T>::DeconstBaseType& DeconstBaseType;
			typedef typename ConstTraits<T>::NonConstType& NonConstType;
			typedef typename ConstTraits<T>::ConstBaseType& ConstBaseType;
		};

		template <typename T>
		struct ConstTraits<T*>
		{
			enum { result = ConstTraits<T>::result, };
			typedef typename ConstTraits<T>::DeconstBaseType* DeconstBaseType;
			typedef typename ConstTraits<T>::NonConstType* NonConstType;
			typedef typename ConstTraits<T>::ConstBaseType* ConstBaseType;
		};

		template <typename T>
		struct ConstTraits<T * const>
		{
			enum { result = ConstTraits<T>::result, };
			typedef typename ConstTraits<T>::DeconstBaseType* const DeconstBaseType;
			typedef typename ConstTraits<T>::NonConstType* NonConstType;
			typedef typename ConstTraits<T>::ConstBaseType* const ConstBaseType;
		};

	public:
		enum 
		{
			isPointer = PointerTraits<T>::isPointer,
			isReference = PointerTraits<T>::isReference,
			baseIsConst = ConstTraits<T>::result,
			ptrcount = PointerTraits<T>::ptrcount,
		};

		typedef typename PointerTraits<T>::PointeeType PointeeType;

		// if T is Object******, this type is Object.
		// if T is Object, this type is Object.
		// if T is Object&***&***, this type is Object.
		typedef typename PointerTraits<T>::BaseType BaseType;

		typedef typename ParameterTraits<T>::result ParameterType;

		// const int**	-> int**
		// const int&	-> int&
		// const int	-> int
		typedef typename ConstTraits<T>::DeconstBaseType DeconstBaseType;
		typedef typename ConstTraits<T>::NonConstType NoConstType;
		typedef typename ConstTraits<T>::ConstBaseType ConstBaseType;

		typedef typename RefTraits<T>::DerefType DereferenceType;
		typedef typename RefTraits<T>::AddrefType AddReferenceType;
	};

	struct PointerValue
	{
		template <typename T>
		static typename TypeTraits<T>::BaseType* ptrValue(T& param)
		{
			return &param;
		}

		template <typename T>
		static inline typename TypeTraits<T>::BaseType* ptrValue(T* param)
		{
			return ptrValue(*param);
		}
	};

	template <typename T, unsigned ptrcount>
	struct FindPtr
	{
		typedef typename FindPtr<T*, ptrcount-1>::result result;
	};

	template <typename T, unsigned ptrcount>
	struct FindPtr<T&, ptrcount>
	{
		typedef typename FindPtr<T*, ptrcount-1>::result result;
	};

	template <typename T>
	struct FindPtr<T, 0>
	{
		typedef T result;
	};

	template <typename T>
	struct Type2Type
	{

	};

}