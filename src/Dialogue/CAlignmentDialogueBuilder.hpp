#pragma once
#include "CBaseDialogueBuilder.hpp"
#include <Engine/irr_ptr.h>

namespace xihad { namespace dialogue
{
	class CDialogueAlignmenter;
	class CAlignmentDialogueBuilder : public CBaseDialogueBuilder
	{
	public:
		CAlignmentDialogueBuilder(ITextElementFactory* factory, CDialogueAlignmenter* alignmenter);

		virtual ~CAlignmentDialogueBuilder();

	protected:
		virtual CAlignedTextSection* onLinkTextSections() override;
		virtual void reset() override;

	private:
		irr_ptr<CDialogueAlignmenter> mAlignmenter;
	};
}}

