#pragma once
#include <Dialogue\ITextContent.hpp>
#include <CEGUI\String.h>
#include "ITypedContent.h"

namespace CEGUI
{
	class Font;
}

namespace xihad { namespace dialogue
{
	class CPlainTextContent : public ITextContent, public ITypedContent
	{
	private:
		static CEGUI::String getWordFollowSymbol();

	public:
		explicit CPlainTextContent(const CEGUI::Font& font, const CEGUI::String& text);
		
		virtual ~CPlainTextContent();

		// inherit from ITextContent
		virtual void getLetterRange( unsigned& bgnIndex, unsigned* endIndex = 0 ) const override;

		virtual void getWordRange( unsigned& bgnIndex, unsigned* endIndex = 0 ) const override;

		virtual ITextContent* split( unsigned index ) override;

		virtual unsigned endIndex() const override;

		virtual bool empty() const override;

		virtual SFillResult fillHorizontal( unsigned widthLimit, bool allowEmpty = true ) const override;
		
		// inherit from ITypedElement
		virtual const CEGUI::String& getType() const;

		// sub class method
		CEGUI::String substr(unsigned bgn, unsigned len = CEGUI::String::npos) const;

		float getWidth(unsigned bgn, unsigned len = CEGUI::String::npos) const;

		float getHeight() const;

	private:
		const CEGUI::Font* mFont;
		CEGUI::String mText;
	};
}}