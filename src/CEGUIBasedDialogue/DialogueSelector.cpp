#include "DialogueSelector.h"

#include <CEGUI\GUIContext.h>
#include <CEGUI\System.h>
#include <CEGUI\Window.h>

namespace xihad { namespace dialogue
{
	using namespace CEGUI;

	const CEGUI::String DialogueSelector::TEXT_AREA = "TextArea";
	const CEGUI::String DialogueSelector::IMAGE_AREA = "ImageArea";
	const CEGUI::String DialogueSelector::CHARACTER_NAME = "CharacterName";
	const CEGUI::String DialogueSelector::CONTAINER = "__auto_container__";
	const CEGUI::String DialogueSelector::NEXT_BUTTON = "__auto_next__";

	DialogueSelector::DialogueSelector(bool startFromLeft /*=true*/)
	{
		GUIContext& context = System::getSingleton().getDefaultGUIContext();
		Window* root = context.getRootWindow();
		left = root->getChild("LeftDialog");
		right = root->getChild("RightDialog");
		left->setVisible(false);
		right->setVisible(false);
		
		current = startFromLeft ? right : left;
	}

	Window* DialogueSelector::nextDialog( const CEGUI::String& name, const CEGUI::String& image )
	{
		Window* textArea = current->getChild(TEXT_AREA);
		if (textArea->getProperty(CHARACTER_NAME) != name)
		{
			switchDialog();
			textArea->fireEvent("CloseSelf", WindowEventArgs(textArea));
		}

		textArea = current->getChild(TEXT_AREA);
		textArea->setProperty(CHARACTER_NAME, name);
		textArea->fireEvent("OpenSelf", WindowEventArgs(textArea));

		Window* imageArea = current->getChild(IMAGE_AREA);
		imageArea->setProperty("Image", image);

		Window* nextButton = textArea->getChild(DialogueSelector::NEXT_BUTTON);
		nextButton->setVisible(false);
		current->setVisible(true);
		return current;
	}

	void DialogueSelector::switchDialog()
	{
		current = current == left ? right : left;
	}

	Window* DialogueSelector::currentDialog() const
	{
		return current;
	}

	CEGUI::Window* DialogueSelector::getTextContainer( CEGUI::Window* dialogPane )
	{
		return dialogPane->getChildRecursive(CONTAINER);
	}

	CEGUI::Window* DialogueSelector::getNextButton( CEGUI::Window* dialogPane )
	{
		return dialogPane->getChildRecursive(NEXT_BUTTON);
	}

}}

