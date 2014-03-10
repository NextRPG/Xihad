#pragma once
#include "Properties.h"

namespace xihad { namespace ngn
{
	class NullProperties : public Properties
	{
	public:
		virtual B getBool  (S key, B defaultValue = B()) const override
		{
			return defaultValue;
		}

		virtual I getInt   (S key, I defaultValue = I()) const override
		{
			return defaultValue;
		}

		virtual F getFloat (S key, F defaultValue = F()) const override
		{
			return defaultValue;
		}

		virtual D getDimen (S key, D defaultValue = D()) const override
		{
			return defaultValue;
		}

		virtual V getVector(S key, V defaultValue = V()) const override
		{
			return defaultValue;
		}

		virtual Q getQuater(S key, Q defaultValue = Q()) const override
		{
			return defaultValue;
		}

		virtual S getString(S key, S defaultValue = S()) const override
		{
			return defaultValue;
		}

		virtual void pushSelf(lua_State* L) const override;

	};
}}

