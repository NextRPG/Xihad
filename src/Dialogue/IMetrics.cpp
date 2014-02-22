#include "IMetrics.hpp"
#include "ITextContent.hpp"
#include <cassert>
#include <utility>

namespace xihad { namespace dialogue 
{
	IMetrics::SFillResult IMetrics::fillHorizontal(
		ITextContent* text, unsigned widthLimit, bool allowFillEmpty) const
	{
		assert(text);
		int bgn = 0, end;

		SFillResult result(0);
		do {
			const wchar_t* word = text->nextWord(bgn, &end).c_str();
			ngn::dimension2du dimen = getDimension(word);
			unsigned expanded = result.width + dimen.Width;
			
			// Need to contain current word
			if (expanded > widthLimit)
				break;

			result.splitIndex = bgn;
			result.width = expanded;
			result.height = std::max(result.height, (int) dimen.Height);

			bgn = end;
		} while(bgn != text->endIndex());

		if (result.splitIndex == 0 && !allowFillEmpty)
		{
			// TODO
		}

		return result;
	}

}}

