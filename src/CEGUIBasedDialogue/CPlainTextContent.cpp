#include "CPlainTextContent.h"
#include "CDialogueContext.h"
#include <CEGUI\TextUtils.h>
#include <CEGUI\FontManager.h>
#include <xutility>
#include <assert.h>
#include <iostream>

using namespace CEGUI;

namespace xihad { namespace dialogue
{
	CEGUI::String CPlainTextContent::WORD_FOLLOW_SYMBOL
		((CEGUI::utf8*)	"·~！@#￥%…&*（）—+{}|：”《》？-=【】、；‘，。、 ");

	CPlainTextContent::CPlainTextContent(const Font& font, const CEGUI::String& text )
		: mFont(&font), mText(text)
	{

	}

	void CPlainTextContent::getLetterRange( unsigned& bgnIndex, unsigned* endIndex /*= 0 */ ) const 
	{
		assert(bgnIndex < mText.length());
		// bgnIndex = bgnIndex;
		if (endIndex) 
			*endIndex = bgnIndex + 1;
	}

	void CPlainTextContent::getWordRange( unsigned& bgnIndex, unsigned* endIndex /*= 0 */ ) const 
	{
		assert(bgnIndex < mText.length());
		// bgnIndex = bgnIndex;
		if (endIndex) 
		{
			CEGUI::String::size_type pos = mText.find_first_not_of(WORD_FOLLOW_SYMBOL, bgnIndex + 1);
			*endIndex = pos == CEGUI::String::npos ? mText.length() : pos;
		}
	}

	ITextContent* CPlainTextContent::split( unsigned index )
	{
		if (index < 1 || index > mText.length()) return nullptr;

		String tail = mText.substr(index);
		mText = mText.substr(0, index);
		return new CPlainTextContent(*mFont, tail);
	}

	unsigned CPlainTextContent::endIndex() const 
	{
		return mText.length();
	}

	bool CPlainTextContent::empty() const 
	{
		return mText.empty();
	}

	ITextContent::SFillResult CPlainTextContent::fillHorizontal
		( unsigned widthLimit, bool allowEmpty /*= true */ ) const 
	{
		unsigned bgn = 0, end;

		SFillResult result = { 0, 0, 0 };
		result.height = (int) getHeight();

		do {
			getWordRange(bgn, &end);
			unsigned expanded = (unsigned) (result.width + getWidth(bgn, end - bgn));

			// Need to contain current word
			if (expanded > widthLimit) 
				break;

			result.splitIndex = end;
			result.width = expanded;
			bgn = end;
		} while(bgn != mText.length());

		if (result.splitIndex == 0 && !allowEmpty)
		{
			result.splitIndex = std::min(1u, mText.length());
			result.width = (int) (result.width + getWidth(0, result.splitIndex - 0));
		}

		return result;
	}

	CEGUI::String CPlainTextContent::substr( unsigned bgn, unsigned len /*= CEGUI::String::npos*/ ) const
	{
		return mText.substr(bgn, len);
	}

	float CPlainTextContent::getWidth( unsigned bgn, unsigned len /*= CEGUI::String::npos*/ ) const
	{
		return mFont->getTextExtent(mText.substr(bgn, len));
	}

	float CPlainTextContent::getHeight() const
	{
		return mFont->getFontHeight();
	}

	const CEGUI::String& CPlainTextContent::getType() const
	{
		const static CEGUI::String type("PlainText");
		return type;
	}

	CPlainTextContent::~CPlainTextContent()
	{
#ifdef _DEBUG
		std:: cout << "CPlainTextContent deleted." << std::endl;
#endif // _DEBUG
	}

}}