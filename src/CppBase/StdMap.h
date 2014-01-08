#pragma once
#include <unordered_map>
#include <boost/call_traits.hpp>

namespace xihad { namespace ngn
{
	class StdMap
	{
	public:
		template<
			class KeyType, class ValueType, 
			class _Hasher, class _Keyeq, 
			class  _Alloc>
		static ValueType* findPtrValue(
			std::unordered_map<KeyType, ValueType*, _Hasher, _Keyeq,  _Alloc>& umap, 
			const KeyType& key,
			ValueType* defaultRet = NULL)
		{
			auto got = umap.find(key);
			ValueType* ret = defaultRet;
			if (got != umap.end())
			{
				ret = got->second;
			}

			return ret;
		}

		template<
			class KeyType, class ValueType, 
			class _Hasher, class _Keyeq, 
			class  _Alloc>
		static ValueType* findValuePtr(
			std::unordered_map<KeyType, ValueType, _Hasher, _Keyeq,  _Alloc>& umap, 
			const KeyType& key)
		{
			auto gotRef = umap.find(key);
			if (gotRef != umap.end())
				return &gotRef->second;
			else
				return NULL;
		}

		template<
			class KeyType, class ValueType, 
			class _Hasher, class _Keyeq,
			class  _Alloc>
		static bool containsKey(
			const std::unordered_map<KeyType, ValueType, _Hasher, _Keyeq,  _Alloc>& umap, 
			const KeyType& key)
		{
			return umap.find(key) != umap.end();
		}

		template<
			class KeyType, class ValueType, 
			class _Hasher, class _Keyeq,
			class  _Alloc>
		inline static void insert(
			std::unordered_map<KeyType, ValueType, _Hasher, _Keyeq,  _Alloc>& umap, 
			const typename boost::call_traits<KeyType>::param_type k,
			const typename boost::call_traits<ValueType>::param_type v)
 		{
			umap.insert(std::pair<
				typename boost::call_traits<KeyType>::param_type, 
				typename boost::call_traits<ValueType>::param_type>(k, v));
		}

	};
}}

