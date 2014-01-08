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
		static int select(const char* target, const char* options[]);

		/// Split source string into substring and push back to result set
		/**
		 * Suppose that the sep is '.' and the esc is '%', then
		 * <pre>
		 *	abc.abc		-> [abc, abc]
		 *	abc%.a.bc	-> [abc.a, bc]
		 *	abc%%a.bc	-> [abc%a, bc]
		 *	abc%ca.bc	-> [abcca, bc]
		 *	abc.abc%	-> [abc, abc%]
		 *	abc..abc	-> [abc, abc]
		 * </pre>
		 * Rules:
		 *	1. Every character following <esc> will be concated directly.
		 *	2. If a <esc> character is in the end of source string, then it will be concated.
		 *	3. <sep> will split the source string into pieces
		 *	4. Empty piece will be ignored if ignoreEmptyPiece is true
		 *	
		 * @param res		All string piece will be push_back() to this container
		 * @param source	Specify The source string which will be splitted into pieces
		 * @param sep		Specify the separator to split string
		 * @param esc		Specify the esc character, can't be the same with <sep>
		 * @param ignEmpty	Specify whether to ignore empty piece
		 */
		template <typename ResultSet, typename charT, typename charTraits, typename charAlloc>
		static int split(ResultSet& res, 
			const std::basic_string<charT, charTraits, charAlloc>& source, 
			charT sep, charT esc, bool ignEmpty = true)
		{
			assert(sep != esc && "The <sep> character can't be same with <esc>");

			int len = source.length(), pieceCount = 0;
			std::string piece;
			bool metEsc = false;
			for (int bgn = 0, end = 0; end < len + 1; ++end)
			{
				// Once traversed the source, force to finish the last piece
				charT ch = end < len ? source[end] : sep;

				// The last <esc> will be concated
				metEsc = end < len ? metEsc : false;
				
				if (metEsc)
				{
					// copy skipping esc
					int endToEsc = end - 1;
					piece += source.substr(bgn, endToEsc-bgn);
					piece += ch;

					metEsc = false;
					bgn = end + 1;
				}
				else if (ch == sep)
				{
					piece += source.substr(bgn, end-bgn);

					// ignore the empty piece
					if (!ignEmpty || !piece.empty()) 
					{
						res.push_back(piece);
						++pieceCount;
						piece = "";
					}

					bgn = end + 1;
				}
				else 
				{
					metEsc = (ch == esc);
				}
			} // end for loop

			return pieceCount;
		}
	

	private:
		template <typename T> 
		struct Type2Type 
		{
			typedef T type;
		};

	public:
		/// The inverse function of split
		template <typename iterator>
		static typename iterator::value_type combine(
			iterator bgn, iterator end, 
			typename Type2Type<typename iterator::value_type>::type::value_type sep, 
			typename Type2Type<typename iterator::value_type>::type::value_type esc, 
			bool optmEmpty = true)
		{
			typedef typename iterator::value_type	 string_type;
			typedef typename string_type::value_type charT;
			assert(sep != esc && "The <sep> character can't be same with <esc>");

			string_type result;
			for (; bgn != end; ++bgn)
			{
				if (optmEmpty && bgn->empty())
					continue;

				for (charT ch : *bgn)
				{
					if (ch == sep || ch == esc)
						result += esc;

					result += ch;
				}
				result += sep;
			}

			// erase the last separator
			result.erase(result.length() - 1);

			return result;
		}

	};
}

