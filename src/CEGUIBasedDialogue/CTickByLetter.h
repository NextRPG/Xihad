#pragma once
#include <Dialogue\CBaseTickMethod.hpp>

namespace xihad { namespace dialogue
{
	class IDialogue;
	class CTickByLetter : public CBaseTickMethod
	{
	public:
		explicit CTickByLetter(IDialogue& target, float standardCycle, float initSpeed = 1.0f);

		virtual ~CTickByLetter();

	protected:
		virtual bool onTick() override;

	private:
		IDialogue& mTarget;
	};
}}
