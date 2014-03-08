#include "CTickByWord.h"
#include <Dialogue\IDialogue.hpp>
#include <Dialogue\ITextElement.hpp>
#include <Dialogue\ITextContent.hpp>

namespace xihad { namespace dialogue
{
	CTickByWord::CTickByWord( IDialogue& target, float standardCycle, float initSpeed /*= 1.0f*/) 
		: CBaseTickMethod(standardCycle, initSpeed), mTarget(target)
	{
		
	}

	bool CTickByWord::onTick()
	{
		auto visibility = mTarget.getVisibility();
		auto elementItr = visibility.endElement;
		auto bgnIdx = visibility.endLetter;
		
		--elementItr;
		if (bgnIdx == (*elementItr)->getContent()->endIndex())
		{
			++elementItr;	// last element is all visible
			bgnIdx = 0;
		}

		if (elementItr == mTarget.endVisibility().endElement)
			return false;		// reach end, no more word to show

		unsigned endIdx;
		do
		{
			(*elementItr)->getContent()->getWordRange(bgnIdx, &endIdx);
			++elementItr;
		} while (endIdx - bgnIdx > 0);
		
		visibility.endElement = elementItr;
		visibility.endLetter = endIdx;
		mTarget.setVisibility(visibility, false);
		return true;
	}

	CTickByWord::~CTickByWord()
	{
#ifdef _DEBUG
		std:: cout << "CTickByWord deleted." << std::endl;
#endif // _DEBUG
	}

}}