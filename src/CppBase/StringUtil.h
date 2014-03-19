#pragma once
#include <string>
#include <cassert>

namespace xihad
{
	/// 字符串算法集合
	class StringUtil
	{
	public:
		/// 查找匹配字符串
		/**
		 * 从字符串数组中找出和目标字符串相等的字符串的索引
		 * 
		 * @param target 指定目标字符串
		 * @param options 指定搜索的字符串数组
		 * @return 找到的字符串索引，或者 -1
		 */
		static int select(const char* target, const char* const options[]);

		static int select(const char* target, 
						  const char* const options[], 
						  const char* eliminatables);

		/// 判断字符串除去某些字符后能否相等
		/**
		 * do
		 *	i = j = 0
		 *	while target[i] == compare[j] or target[i++] is eliminatable
		 * end
		 * ("abcd efg", "abcdefg", " ") -> 1
		 */
		static int equal_except(const char* target, 
								const char* compare, 
								const char* eliminatables);
	};
}

