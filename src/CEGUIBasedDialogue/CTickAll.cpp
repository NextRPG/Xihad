#include "CTickAll.h"
#include <Dialogue\IDialogue.hpp>

namespace xihad { namespace dialogue
{
	CTickAll::CTickAll( IDialogue& target) : mTarget(target)
	{

	}

	void CTickAll::tick( float delta )
	{
		auto end = mTarget.endVisibility();
		auto current = mTarget.getVisibility();
		if (current.endElement == end.endElement && current.endLetter == end.endLetter)
			return;

		mTarget.setVisibility(mTarget.endVisibility(), true);
	}

}}
