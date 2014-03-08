#pragma once
#include <Dialogue\IDialogueContext.hpp>

namespace CEGUI
{
	class Font;
	class Window;
}

namespace xihad { namespace dialogue
{
	class CDialogueContext : public IDialogueContext
	{
	public:
		explicit CDialogueContext(CEGUI::Window& parent);

		virtual ~CDialogueContext();

		virtual ITextElement* create( ITextContent* content );

	private:
		CDialogueContext(const CDialogueContext&);
		CDialogueContext& operator=(const CDialogueContext&);

	private:
		CEGUI::Window* mBaseWindow;
	};
}}

