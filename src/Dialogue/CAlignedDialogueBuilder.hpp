#pragma once
#include "CBaseDialogueBuilder.hpp"
#include <Engine/irr_ptr.h>

namespace xihad { namespace dialogue
{
	class CDialogueAlignmenter;
	class CAlignedDialogueBuilder : public CBaseDialogueBuilder
	{
	public:
		CAlignedDialogueBuilder(IDialogueContext* factory, CDialogueAlignmenter* alignmenter);

		virtual ~CAlignedDialogueBuilder();

	protected:
		virtual CAlignedTextSection* onLinkTextSections() override;
		virtual void reset() override;

	private:
		irr_ptr<CDialogueAlignmenter> mAlignmenter;
	};
}}

