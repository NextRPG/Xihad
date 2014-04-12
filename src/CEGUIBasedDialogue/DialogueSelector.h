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
		static const CEGUI::String LEFT_DIALOG;
		static const CEGUI::String RIGHT_DIALOG;
		
		static const CEGUI::String IMAGE_AREA;
		static const CEGUI::String TEXT_AREA;
		static const CEGUI::String SUBTITLE_CONTAINER;
		static const CEGUI::String NEXT_BUTTON;

		static const CEGUI::String CHARACTER_NAME_PROPERTY;
		static const CEGUI::String PORTRAIT_PROPERTY;

		static const CEGUI::String MAGNIFY_ANIM_EVENT;
		static const CEGUI::String SHRINK_ANIM_EVENT;

		static const CEGUI::String LIGHTER_ANIM_EVENT;
		static const CEGUI::String DARKER_ANIM_EVENT;

		static const CEGUI::String HIDE_EVENT;

	public:
		DialogueSelector(bool startFromLeft);

		CEGUI::Window* nextDialog(const CEGUI::String& name, const CEGUI::String& image);

		CEGUI::Window* currentDialog() const;		
		
		void close();

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

