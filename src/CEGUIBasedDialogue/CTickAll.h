#pragma once
#include <Dialogue\CBaseTickMethod.hpp>

namespace xihad { namespace dialogue
{
	class IDialogue;
	class CTickAll : public CBaseTickMethod
	{
	public:
		explicit CTickAll(IDialogue& target, float standardCycle, float initSpeed = 1.0f);

		virtual ~CTickAll();

	protected:
		virtual bool onTick() override;

	private:
		IDialogue& mTarget;
	};

}}
