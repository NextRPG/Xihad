#pragma once
#include <vector>
#include <Engine/irr_ptr.h>

namespace xihad { namespace dialogue 
{
	class ITickMethod;
	class ITextElement;
	class IDialogueContext;

	class IDialogue
	{
	public:
		typedef std::vector<irr_ptr<ITextElement>> TextElements;
		struct SDialogueVisibility
		{
			typedef TextElements::const_iterator ElementIter;
			typedef unsigned LetterIter;

			SDialogueVisibility(ElementIter e = ElementIter(), LetterIter l = 0) :
				endElement(e), endLetter(l)
			{
			}

			bool operator==(const SDialogueVisibility& other) const
			{
				return endElement == other.endElement && endLetter == other.endLetter;
			}

			TextElements::const_iterator endElement;	// after current visible element
			LetterIter endLetter;						// after current visible letter
		};

	public:
		virtual ~IDialogue() {}


		virtual void setEventEnabled(bool enable) = 0;
		virtual bool isEventEnabled() const = 0;
		virtual void setTickMethod(ITickMethod* method) = 0;
		virtual ITickMethod* getTickMethod() const = 0;
		virtual IDialogueContext* getContext() const = 0;
		virtual void onUpdate(float deltaTime) = 0;

		// Tick interface
		virtual void setVisibility(const SDialogueVisibility& pVisibility, bool immediate) = 0;
		virtual SDialogueVisibility getVisibility() const = 0;
		virtual SDialogueVisibility beginVisibility() const = 0;
		virtual SDialogueVisibility endVisibility() const = 0;
	};
}}
