#include <algorithm>
#include "CPlainTextElement.h"
#include "CPlainTextContent.h"

#include <CEGUI\Window.h>
#include <CEGUI\CoordConverter.h>

using namespace CEGUI;
namespace xihad { namespace dialogue
{
	CPlainTextElement::CPlainTextElement(Window* container, CPlainTextContent* content)
		: mContainer(container), mContent(content)
	{
		assert(container && content);
		resizeToContent();
	}

	void CPlainTextElement::resizeToContent()
	{
		Sizef parentSize = mContainer->getParentPixelSize();
		float wScale = CoordConverter::asRelative(UDim(0, mContent->getWidth(0)), parentSize.d_width);
		float yScale = CoordConverter::asRelative(UDim(0, mContent->getHeight()), parentSize.d_height);
		mContainer->setSize(USize(UDim(wScale, 0), UDim(yScale, 0)));
	}

	void CPlainTextElement::setVisible( unsigned bgnIndex, unsigned endIndex )
	{
		auto visible = mContent->substr(bgnIndex, endIndex - bgnIndex);
		mContainer->setText(visible);
	}

	void CPlainTextElement::setOffset( ngn::position2di pos )
	{
		Sizef parentSize = mContainer->getParentPixelSize();
		float xScale = CoordConverter::asRelative(UDim(0, (float) pos.X), parentSize.d_width);
		float yScale = CoordConverter::asRelative(UDim(0, (float) pos.Y), parentSize.d_height);
		mContainer->setPosition(UVector2(UDim(xScale, 0), UDim(yScale, 0)));
	}

	const ITextContent* CPlainTextElement::getContent() const
	{
		return mContent.get();
	}

	CPlainTextElement::~CPlainTextElement()
	{
#ifdef _DEBUG
		std:: cout << "CPlainTextElement deleted." << std::endl;
#endif // _DEBUG
	}

}}