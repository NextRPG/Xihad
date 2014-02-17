#include "CBaseDialogueBuilder.hpp"
#include "ITextElementFactory.hpp"

using namespace irr;
using namespace gui;
namespace xihad { namespace dialogue
{


	ITextElement* CBaseDialogueBuilder::createTextElement(
		const std::string& fontPath, const std::wstring& txt)
	{
		IGUIFont* font = mFactory->getFont(fontPath);
		return mFactory->create(font, txt);
	}

}}
