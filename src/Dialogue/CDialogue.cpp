#include "CDialogue.hpp"
#include "ITextElement.hpp"

namespace xihad { namespace dialogue 
{
	CDialogue::CDialogue( TextElements& elements, TickEvents& events ) :
		mTickMethod(0)
	{
		mEvents.splice(mEvents.end(), events);
		mTextElements.splice(mTextElements.end(), elements);
	}

	CDialogue::~CDialogue()
	{
	}

}}
