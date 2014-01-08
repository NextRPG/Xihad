#pragma once
#include "null_type.h"

#define LUAT_TUPLE_1(A1) luaT::tuple<A1, luaT::FinishType>
#define LUAT_TUPLE_2(A1, A2) luaT::tuple<A1, LUAT_TUPLE_1(A2)>
#define LUAT_TUPLE_3(A1, A2, A3) luaT::tuple<A1, LUAT_TUPLE_2(A2, A3)>
#define LUAT_TUPLE_4(A1, A2, A3, A4) luaT::tuple<A1, LUAT_TUPLE_3(A2, A3, A4)>
#define LUAT_TUPLE_5(A1, A2, A3, A4, A5) luaT::tuple<A1, LUAT_TUPLE_4(A2, A3, A4, A5)>
#define LUAT_TUPLE_6(A1, A2, A3, A4, A5, A6) luaT::tuple<A1, LUAT_TUPLE_5(A2, A3, A4, A5, A6)>
#define LUAT_TUPLE_7(A1, A2, A3, A4, A5, A6, A7) luaT::tuple<A1, LUAT_TUPLE_6(A2, A3, A4, A5, A6, A7)>
#define LUAT_TUPLE_8(A1, A2, A3, A4, A5, A6, A7, A8) luaT::tuple<A1, LUAT_TUPLE_7(A2, A3, A4, A5, A6, A7, A8)>
#define LUAT_TUPLE_9(A1, A2, A3, A4, A5, A6, A7, A8, A9) luaT::tuple<A1, LUAT_TUPLE_8(A2, A3, A4, A5, A6, A7, A8, A9)>

namespace luaT { 

	struct FinishType 
	{
	};

	template <typename Head, typename Tail>
	struct tuple 
	{
		Head value;
		Tail rests;	// rest value.
	};

	/************************************************************************/
	/* A Sample code to create functions making use of TypeList.
	 * 
	 *	template <typename Head, typename Tail>
	 *	void function(TypeList<Head, Tail>& arg)
	 *	{
	 *		cout << arg.value << " ";
	 *		function(arg.rests);
	 *	}
	 * 
	 *	void function(FinishType e)
	 *	{
	 *		cout << endl;
	 *	}
	/************************************************************************/

	// Helper class to create access Typelist data.

	template <typename T>
	struct ListSize { };

	template <typename H, typename T>
	struct ListSize<tuple<H, T> >
	{
		enum { result = ListSize<T>::result+1, };
	};

	template <>
	struct ListSize<FinishType>
	{
		enum { result = 0, };
	};

	template <int idx, typename T>
	struct ElementAt 
	{
		typedef NullType result;
	};

	template <int idx, typename Head, typename Tail>
	struct ElementAt<idx, tuple<Head, Tail> >
	{
		typedef typename ElementAt<idx-1, Tail>::result result;
	};

	template <int idx>
	struct ElementAt<idx, FinishType>
	{
		typedef NullType result;
	};

	template <typename Head, typename Tail>
	struct ElementAt<0, tuple<Head, Tail>>
	{
		typedef Head result;
	};

	template <int i>
	struct Int2Type 
	{
	};

	template <int idx, typename H, typename T>
	inline typename ElementAt<idx, tuple<H, T>>::result getTuple(const tuple<H, T>& tl)
	{
		return getTuple(Int2Type<idx>(), tl);
	}

	template <int idx, typename H, typename T>
	inline typename ElementAt<idx, tuple<H, T>>::result getTuple(Int2Type<idx>, const tuple<H, T>& tl)
	{
		return getTuple(Int2Type<idx-1>(), tl.rests);
	}

	template <typename H, typename T>
	inline typename ElementAt<0, tuple<H, T>>::result getTuple(Int2Type<0>, const tuple<H, T>& tl)
	{
		return tl.value;
	}

	//////////////////////////////////////////////////////////////////////////
	// Setter Functions.
	//////////////////////////////////////////////////////////////////////////

	template <int idx, typename H, typename T>
	static inline void setTuple_(Int2Type<idx>, tuple<H, T>& tl, typename ElementAt<idx, tuple<H, T>>::result value)
	{
		return setTuple_(Int2Type<idx-1>(), tl.rests, value);
	}

	template <typename H, typename T>
	static inline void setTuple_(Int2Type<0>, tuple<H, T>& tl, typename ElementAt<0, tuple<H, T>>::result value)
	{
		tl.value = value;
	}

	template <int idx, typename H, typename T>
	inline void setTuple(tuple<H, T>& tl, typename ElementAt<idx, tuple<H, T>>::result value)
	{
		return setTuple_(Int2Type<idx>(), tl, value);
	}

}

