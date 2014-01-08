#include "MapProperties.h"
#include "CppBase\StdMap.h"
#include <boost\variant.hpp>

namespace xihad { namespace ngn
{
	template <typename ValueType>
	ValueType* getValue(const MapProperties::PropertyContainer& map, Properties::S key)
	{
		auto noConst = const_cast<MapProperties::PropertyContainer&>(map);
		if (Property* prop = StdMap::findValuePtr(noConst, std::string(key)))
		{
			auto b = boost::get<ValueType>(prop);
			return b;
		}
		else
		{
			return nullptr;
		}
	}

	Properties::B MapProperties::getBool  ( S key, B defaultValue ) const
	{
		if (auto b = getValue<bool>(*this, key))
			return *b;
		else
			return defaultValue;
	}

	Properties::I MapProperties::getInt   ( S key, I defaultValue ) const
	{
		if (auto b = getValue<I>(*this, key))
			return *b;
		else
			return defaultValue;
	}

	Properties::F MapProperties::getFloat ( S key, F defaultValue ) const
	{
		if (auto b = getValue<F>(*this, key))
			return *b;
		else
			return defaultValue;
	}

	Properties::D MapProperties::getDimen ( S key, D defaultValue ) const
	{
		if (auto b = getValue<D>(*this, key))
			return *b;
		else
			return defaultValue;
	}

	Properties::V MapProperties::getVector( S key, V defaultValue ) const
	{
		if (auto b = getValue<V>(*this, key))
			return *b;
		else
			return defaultValue;
	}

	Properties::Q MapProperties::getQuater( S key, Q defaultValue ) const
	{
		if (auto b = getValue<Q>(*this, key))
			return *b;
		else
			return defaultValue;
	}

	Properties::S MapProperties::getString( S key, S defaultValue ) const
	{
		if (auto b = getValue<std::string>(*this, key))
			return b->c_str();
		else
			return defaultValue;
	}

	void MapProperties::pushSelf( lua_State* L ) const
	{
		// TODO: not implemented
		throw std::exception("This method hasn't been implemented");
	}

}}
