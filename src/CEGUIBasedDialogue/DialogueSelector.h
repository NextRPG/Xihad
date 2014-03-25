#pragma once

namespace CEGUI
{
	class Window;
	class String;
}

namespace xihad { namespace dialogue
{
	class DialogueSelector
	{
	private:
		static const CEGUI::String IMAGE_AREA;
		static const CEGUI::String TEXT_AREA;
		static const CEGUI::String CHARACTER_NAME;
		static const CEGUI::String CONTAINER;
		static const CEGUI::String NEXT_BUTTON;
	public:
		DialogueSelector(bool startFromLeft);

		CEGUI::Window* nextDialog(const CEGUI::String& name, const CEGUI::String& image);

		CEGUI::Window* currentDialog() const;		

		static CEGUI::Window* getTextContainer(CEGUI::Window* dialogPane);

		static CEGUI::Window* getNextButton(CEGUI::Window* dialogPane);

	private:
		void switchDialog();

	private:
		CEGUI::Window* left;
		CEGUI::Window* right;
		CEGUI::Window* current;
	};
}}

