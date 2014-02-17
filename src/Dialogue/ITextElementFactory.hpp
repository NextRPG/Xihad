#pragma once
#include <string>
#include <irrlicht/IReferenceCounted.h>

namespace irr { namespace gui 
{
	class IGUIFont;
}}
	 
namespace xihad { namespace dialogue 
{
	class ITextElement;
	class ITextElementFactory : public virtual irr::IReferenceCounted
	{
	public:	
		virtual ~ITextElementFactory() {}

		virtual irr::gui::IGUIFont* getFont(const std::string& fontPath) = 0;

		virtual ITextElement* create(irr::gui::IGUIFont* font, const std::wstring& txt) = 0;
	};

}}

