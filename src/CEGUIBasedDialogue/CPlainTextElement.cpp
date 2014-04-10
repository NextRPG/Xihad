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
		mContainer->setSize(USize(UDim(0, mContent->getWidth(0)), UDim(0, mContent->getHeight())));
	}

	void CPlainTextElement::setVisible( unsigned bgnIndex, unsigned endIndex )
	{
		auto visible = mContent->substr(bgnIndex, endIndex - bgnIndex);
		mContainer->setText(visible);
	}

	void CPlainTextElement::setOffset( ngn::position2di pos )
	{
		mContainer->setPosition(UVector2(UDim(0, (float) pos.X), UDim(0, (float) pos.Y)));
	}

	const ITextContent* CPlainTextElement::getContent() const
	{
		return mContent.get();
	}

	CPlainTextElement::~CPlainTextElement()
	{
		Window* parent = mContainer->getParent();
		parent->removeChild(mContainer);
#ifdef DEBUG_DIALOG
		std:: cout << "CPlainTextElement deleted." << std::endl;
#endif // DEBUG_DIALOG
	}

}}