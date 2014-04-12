#include "CTickByLetter.h"
#include <Dialogue\IDialogue.hpp>
#include <Dialogue\ITextElement.hpp>
#include <Dialogue\ITextContent.hpp>
#include <assert.h>

namespace xihad { namespace dialogue
{
	CTickByLetter::CTickByLetter( IDialogue& target, float standardCycle, float initSpeed /*= 1.0f*/) 
		: CBaseTickMethod(standardCycle, initSpeed), mTarget(target)
	{
		
	}

	bool CTickByLetter::onTick()
	{
		auto visibility = mTarget.getVisibility();
		if (visibility == mTarget.endVisibility())
			return false;

		auto bgnElement = visibility.endElement;
		auto bgnIdx = visibility.endLetter;
		if (visibility == mTarget.beginVisibility())
		{
			assert(bgnIdx == 0);
		}
		else
		{
			--bgnElement;
		}

		auto endIdx = (*bgnElement)->getContent()->endIndex();
		if (bgnIdx == endIdx)
		{
			assert(endIdx != 0);	// empty element shouldn't be exist.
			++bgnElement;	// last element is all visible
			bgnIdx = 0;
		}

		(*bgnElement)->getContent()->getLetterRange(bgnIdx, &endIdx);
		assert(endIdx > bgnIdx);
		
		mTarget.setVisibility(IDialogue::SDialogueVisibility(++bgnElement, endIdx), false);
		return true;
	}

}}