#pragma once
#include <Dialogue\ITextElement.hpp>
#include <CEGUI\String.h>
#include <Engine\irr_ptr.h>

namespace CEGUI
{
	class Window;
}

namespace xihad { namespace dialogue
{
	class CPlainTextContent;
	class CPlainTextElement : public ITextElement
	{
	public:
		explicit CPlainTextElement(CEGUI::Window* container, CPlainTextContent* content);

		virtual ~CPlainTextElement();

		virtual void setVisible( unsigned bgnIndex, unsigned endIndex ) override;

		virtual void setOffset( ngn::position2di pos ) override;

		virtual const ITextContent* getContent() const;

	private:
		void resizeToContent();

		CPlainTextElement(const CPlainTextElement&);
		
		CPlainTextElement& operator=(const CPlainTextElement&);

	private:
		CEGUI::Window* mContainer;
		irr_ptr<CPlainTextContent> mContent;
	};
}}