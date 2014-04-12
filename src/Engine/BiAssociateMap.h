#pragma once

namespace xihad { namespace ngn
{
	/// 双向关联键值对，值必须唯一
	/**
	 * K2V 表示键值对，可以是 map, multimap, unorderer_map 等。
	 * V2KIter 表示从 value 映射到 k-v'iterator ，可以实现快速删除。
	 */
	template <typename K, typename V, typename K2V, typename V2KIter>
	class BiAssociateMap
	{
	public:
		void add(const K& key, const V& value)
		{
			typename V2KIter::iterator keyIterPos = value2KeyPos.find(value);
			if (keyIterPos != value2KeyPos.end())
				key2Value.erase(keyIterPos->second);

			typename K2V::iterator pos = key2Value.insert(std::pair<K, V>(key, value));
			value2KeyPos[value] = pos;
		}

		void remove(const V& value)
		{
			typename V2KIter::iterator keyIterPos = value2KeyPos.find(value);
			if (keyIterPos != value2KeyPos.end())
			{
				typename K2V::iterator pos = keyIterPos->second;
				key2Value.erase(pos);
				value2KeyPos.erase(keyIterPos);
			}
		}

		K2V& pairs()
		{
			return key2Value;
		}

	private:
		K2V key2Value;
		V2KIter value2KeyPos;
	};
}}
