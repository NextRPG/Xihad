#include "CDialogueContext.h"
#include <CEGUI\Window.h>
#include <Dialogue\ITextContent.hpp>
#include <assert.h>

#include "ITypedContent.h"
#include "CPlainTextElement.h"
#include "CPlainTextContent.h"

using namespace CEGUI;
namespace xihad { namespace dialogue
{
	CDialogueContext::CDialogueContext( Window& parent ) : mBaseWindow(&parent)
	{

	}

	ITextElement* CDialogueContext::create( ITextContent* content )
	{
		ITypedContent* element = dynamic_cast<ITypedContent*>(content);
		if (element == nullptr) 
			return nullptr;

		auto& type = element->getType();
		if (type == "PlainText")
		{
			auto plainText = dynamic_cast<CPlainTextContent*>(content);
			assert(plainText);

			Window* container = mBaseWindow->createChild("Generic/Label");
			return new CPlainTextElement(container, plainText);
		}
		// else if (type == "Image")

		return nullptr;
	}

	CDialogueContext::~CDialogueContext()
	{
#ifdef _DEBUG
		std:: cout << "CDialogContext deleted." << std::endl;
#endif // _DEBUG
	}
}}