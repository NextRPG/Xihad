#pragma once
#include <string>

namespace xihad
{
	template <typename charT>
	class CLineBreak
	{
	public:
		class CLineBreakIterator
		{
		public:
			CLineBreakIterator(const std::basic_string<charT>& str) :
				string(str), bgn(0), end(-1)
			{
				nextLineBreak();
			}

			CLineBreakIterator(const std::basic_string<charT>& str, int) :
				string(str), bgn(str.length()+1), end(bgn)
			{
			}

			CLineBreakIterator& operator++()
			{
				if (bgn <= string.length())
				{
					if (end+1 < string.length() && string[end] == charT('\r') && string[end+1] == charT('\n'))
						++end;

					nextLineBreak();
				}
				else
				{
					throw std::exception("iterator has moved to end");
				}
				
				return *this;
			}

			inline std::basic_string<charT> operator*()
			{
				return string.substr(bgn, end-bgn);
			}

			inline bool operator!=(const CLineBreakIterator& other) const
			{
				return !((&this->string == &other.string) &&
					(this->bgn == other.bgn) && (this->end == other.end));
			}

		private:
			inline void nextLineBreak()
			{
				unsigned len = string.length();
				unsigned idx;
				for (idx=end+1; idx<len; ++idx)
				{
					charT ch = string[idx];
					if (ch == charT('\r') || ch == charT('\n'))
						break;
				}

				bgn = end+1;
				end = idx;
			}

		private:
			const std::basic_string<charT>& string;
			unsigned bgn;
			unsigned end;
		};

		typedef std::basic_string<charT> stringT;
		typedef CLineBreakIterator iterator;

	public:
		explicit CLineBreak(const stringT& src) :
			mSource(src)
		{
		}

		inline iterator begin() const
		{
			return iterator(mSource);
		}

		inline iterator end() const
		{
			int END_SELECTOR = 0;
			return iterator(mSource, END_SELECTOR);
		}

	private:
		stringT mSource;
	};

	template <typename charT>
	CLineBreak<charT> linesOf(const std::basic_string<charT>& src)
	{
		return CLineBreak<charT>(src);
	}
}

