#pragma once
#include <Dialogue\ITickMethod.hpp>

namespace xihad { namespace dialogue
{
	class IDialogue;
	class CTickAll : public ITickMethod
	{
	public:
		explicit CTickAll(IDialogue& target);

		virtual ~CTickAll();

		virtual void setTickSpeed( float speed ) override { }

		virtual float getTickSpeed() const override
		{
			return 100000000.f;
		}

		virtual void tick( float delta ) override;

	private:
		IDialogue& mTarget;
	};

}}
