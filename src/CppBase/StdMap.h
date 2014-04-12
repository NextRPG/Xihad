#pragma once
#include <unordered_map>
#include <boost/call_traits.hpp>

namespace xihad 
{
	class StdMap
	{
	public:
		template<typename Map>
		inline static typename Map::mapped_type findPtrValue(
			Map& umap, const typename Map::key_type& key, typename Map::mapped_type defaultRet = NULL)
		{
			typename Map::iterator got = umap.find(key);
			if (got != umap.end()) 
				return got->second;

			return defaultRet;
		}

		template<typename Map>
		inline static typename Map::mapped_type* findValuePtr(
			Map& umap, const typename Map::key_type& key)
		{
			typename Map::iterator gotRef = umap.find(key);
			if (gotRef != umap.end())
				return &gotRef->second;
			return 0;
		}

		template<typename Map>
		inline static const typename Map::mapped_type* findValuePtr(
			const Map& umap, const typename Map::key_type& key)
		{
			typename Map::key_type gotRef = umap.find(key);
			if (gotRef != umap.end())
				return &gotRef->second;
			
			return 0;
		}

		template<typename Map>
		inline static bool containsKey(const Map& umap, const typename Map::key_type& key)
		{
			return umap.find(key) != umap.end();
		}

		template<typename Map>
		inline static void insert(
			Map& umap, 
			const typename boost::call_traits<typename Map::key_type>::param_type k,
			const typename boost::call_traits<typename Map::mapped_type>::param_type v)
 		{
			umap.insert(std::pair<
				typename boost::call_traits<typename Map::key_type>::param_type, 
				typename boost::call_traits<typename Map::mapped_type>::param_type>(k, v));
		}

	};
}

