#include "CTickAll.h"
#include <Dialogue\IDialogue.hpp>

namespace xihad { namespace dialogue
{
	CTickAll::CTickAll( IDialogue& target, float standardCycle, float initSpeed /* = 1.0f*/ ) 
		: CBaseTickMethod(standardCycle, initSpeed), mTarget(target)
	{

	}

	bool CTickAll::onTick()
	{
		auto end = mTarget.endVisibility();
		auto current = mTarget.getVisibility();
		if (current.endElement == end.endElement && current.endLetter == end.endLetter)
			return false;
		
		mTarget.setVisibility(mTarget.endVisibility(), true);
		return true;
	}

	CTickAll::~CTickAll()
	{
#ifdef _DEBUG
		std:: cout << "CTickAll deleted." << std::endl;
#endif // _DEBUG
	}

}}
