#pragma once

namespace xihad { namespace dialogue 
{
	class ITickMethod;
	class IDialogueContext;
	class IDialogue
	{
	public:
		virtual ~IDialogue() {}

		virtual void setTickMethod(ITickMethod* method) = 0;
		virtual ITickMethod* getTickMethod() const = 0;
		virtual IDialogueContext* getContext() const = 0;
	};
}}

