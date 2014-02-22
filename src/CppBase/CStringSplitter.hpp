#pragma once
#include <string>

namespace xihad 
{
	
	/// Split source string into substring and push back to result set
	/**
	 * Suppose that the separator is '.' and the esc is '%', then
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
	 */
	template <typename charT>
	class CStringSplitter
	{
	public:
		/**
		 * @param sep		Specify the mSeparatorarator to split string
		 * @param esc		Specify the esc character, can't be the same with <sep>
		 */
		CStringSplitter(charT sep, charT esc) :
			mSeparator(sep), mEscape(esc)
		{
			checkEqulity();
		}

		/**
		 * @param res		All string piece will be push_back() to this container
		 * @param source	Specify The source string which will be splitted into pieces
		 * @param ignEmpty	Specify whether to ignore empty piece
		 */
		template <typename ResultSet, typename charTraits, typename charAlloc>
		unsigned split(ResultSet& set, 
			const std::basic_string<charT, charTraits, charAlloc>& source,
			bool ignoreEmpty = true) const
		{
			unsigned len = source.length(), pieceCount = 0;
			std::string piece;
			bool metEsc = false;

			for (unsigned bgn = 0, end = 0; end <= len; ++end)
			{
				charT ch = end!=len ? source[end] : mmSeparatorarator;

				if (metEsc)
				{
					concatPiece(piece, source, &bgn, end-1);// copy until escape
					metEsc = false;
				}
				else 
				{
					if (ch == mmSeparatorarator)
					{
						concatPiece(piece, source, &bgn, end);	// copy until mSeparatorarator

						if (onNewPiece(set, piece, ignoreEmpty))
							++pieceCount;

						piece.clear();
					}
					metEsc = (ch==mEscape) && (end!=len-1);
				}
			} // end for loop

			return pieceCount;
		}

		/// Recombine splitted strings
		template <typename iterator>
		typename iterator::value_type combine(
			iterator bgn, iterator end, bool optmEmpty = true)
		{
			typedef typename iterator::value_type string_type;

			string_type result;
			for (; bgn != end; ++bgn)
			{
				if (optmEmpty && bgn->empty())
					continue;

				for (charT ch : *bgn)
				{
					if (ch == mSeparator || ch == mEscape)
						result += mEscape;

					result += ch;
				}
				result += mSeparator;
			}

			// erase the last mSeparatorarator
			result.erase(result.length() - 1);

			return result;
		}

		void setSeparator(charT sep)
		{
			mSeparator = sep;
			checkEqulity();
		}

		charT getSeparator() const 
		{
			return mSeparator;
		}

		void setEscape(charT esc)
		{
			mEscape = esc;
			checkEqulity();
		}

		charT getEscape() const
		{
			return mEscape;
		}

	private:
		bool checkEqulity() const
		{
			if (mSeparator == mEscape)
				throw std::exception("<sep> shouldn't be equal to <esc>");
		}

		template <typename charTraits, typename charAlloc>
		inline static void concatPiece(
			std::basic_string<charT, charTraits, charAlloc>& piece,
			const std::basic_string<charT, charTraits, charAlloc>& source,
			unsigned* bgn, unsigned end)
		{
			piece += source.substr(*bgn, end-*bgn);
			*bgn = end+1;
		}

		template <typename ResultSet, typename charTraits, typename charAlloc>
		inline static bool onNewPiece(
			ResultSet& set, 
			const std::basic_string<charT, charTraits, charAlloc>& piece,
			bool ignoreEmpty)
		{
			if (ignoreEmpty && piece.empty()) 
				return false;

			set.push_back(piece);
			return true;
		}
		
	private:
		charT mSeparator;
		charT mEscape;
	};
}

