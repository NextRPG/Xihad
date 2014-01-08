#pragma once
#include "CppBase/xassert.h"

namespace xihad
{
	class BitOperation
	{
	public:
		static bool test(int testValue, int pos)
		{
			xassert(pos >= 1 && pos <= 32);
			return (testValue & (1 << (pos-1))) != 0;
		}

		static void set_1(int& value, int pos)
		{
			xassert(pos >= 1 && pos <= 32);
			value |= (1 << (pos-1));
		}

		static void set_0(int& value, int pos)
		{
			xassert(pos >= 1 && pos <= 32);
			value &= (~(1 << (pos-1)));
		}

		static void set(int& value, int pos, bool _1_not_0)
		{
			_1_not_0 ? set_1(value, pos) : set_0(value, pos);
		}
	};
}

