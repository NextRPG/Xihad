#pragma once
#include "xassert.h"

namespace xihad
{
	class XiAssert
	{
	public:
		inline static void isTrue(bool exact)
		{
			xassert(exact);
		}

		inline static void isFalse(bool exact)
		{
			xassert(!exact);
		}

		template <typename T>
		inline static void areEqual(T expect, T exact)
		{
			xassert(expect == exact);
		}

		inline static void notNull(void* ptr)
		{
			xassert(ptr != nullptr);
		}
	};
}
