#pragma once
#include "irrTypes.h"

namespace irr { namespace core
{
	class CRandomizer
	{
	public:
		explicit CRandomizer(s32 seed = 0x0f0f0f0f) :
			Seed(seed)
		{
		}

		void setSeed(s32 seed)
		{
			Seed = seed;
		}

		s32 getSeed() const
		{
			return Seed;
		}

		s32 rand(s32 seed2)
		{
			seed2 |= Seed;

			seed2 = a * (seed2%q) - r* (seed2/q);
			seed2 &= 0x7fffffff;
			return seed2;
		}

	private:
		static const s32 m = 2147483399;	// a non-Mersenne prime
		static const s32 a = 40692;			// another spectral success story
		static const s32 q = m/a;
		static const s32 r = m%a;			// again less than q
		s32 Seed;
	};
}}

