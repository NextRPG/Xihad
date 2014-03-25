#pragma once
#include <unordered_map>
#include "Property.h"
#include "Properties.h"
#include <string>

namespace xihad { namespace ngn
{
	class MapProperties : public Properties, 
		private std::unordered_map<std::string, Property>
	{
	public:
		typedef std::unordered_map<std::string, Property> PropertyContainer;

		using PropertyContainer::operator [];
		using PropertyContainer::find;
		using PropertyContainer::end;
		using PropertyContainer::begin;
		using PropertyContainer::insert;
		using PropertyContainer::at;

		virtual B getBool  (S key, B pDefault = B()) const override;

		virtual I getInt   (S key, I pDefault = I()) const override;

		virtual F getFloat (S key, F pDefault = F()) const override;

		virtual D getDimen (S key, D pDefault = D()) const override;

		virtual V getVector(S key, V pDefault = V()) const override;

		virtual Q getQuater(S key, Q pDefault = Q()) const override;

		virtual S getString(S key, S pDefault = S()) const override;

		virtual void pushSelf(lua_State* L) const override;
	};
}}

