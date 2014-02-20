#include "CDialogue.hpp"
#include "ITextElement.hpp"
#include "ITickMethod.hpp"

namespace xihad { namespace dialogue 
{
	CDialogue::CDialogue( TextElements& elements, TickEvents& events )
	{
		mEvents.splice(mEvents.end(), events);
		mTextElements.splice(mTextElements.end(), elements);
	}

	CDialogue::~CDialogue()
	{
	}

	void CDialogue::onUpdate( float deltaTime )
	{
		mTickMethod->tick(deltaTime);
	}

}}
