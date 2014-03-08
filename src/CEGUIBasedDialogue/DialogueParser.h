#pragma once

namespace CEGUI
{
	class Window;
}
namespace xihad { namespace dialogue
{
	class IDialogueBuilder;
	class IDialogue;
	class DialogueParser
	{
	public:
		explicit DialogueParser();

		virtual ~DialogueParser();

		void initialise();

		IDialogue* generateDialogue();

	private:
		DialogueParser(const DialogueParser&);
		DialogueParser& operator=(const DialogueParser&);

	private:
		IDialogueBuilder* mBuilder;
		CEGUI::Window* mBaseWindow;
	};
}}
