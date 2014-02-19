#pragma once
#include <Engine/irr_ptr.h>

namespace xihad { namespace dialogue 
{
	class ITextAppearance;
	class ITextContent;
	class CTextSection
	{
	public:
		CTextSection(const ITextAppearance* apperance = 0, ITextContent* content = 0);

		~CTextSection();

		const ITextAppearance* getApperance() const
		{
			return mApperance.get();
		}

		void setApperance(const ITextAppearance* newApperance);

		ITextContent* getContent() const
		{
			return mContent.get();
		}

		void split(CTextSection& rest, unsigned index);

	private:
		irr_ptr<const ITextAppearance> mApperance;
		irr_ptr<ITextContent> mContent;
	};
}}

