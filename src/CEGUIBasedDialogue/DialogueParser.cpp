#include <algorithm>
#include "DialogueParser.h"
#include <CEGUI\SchemeManager.h>
#include <CEGUI\WindowManager.h>
#include <CEGUI\GUIContext.h>
#include <CEGUI\Window.h>

#include <Dialogue\CAlignedDialogueBuilder.hpp>
#include <Dialogue\CDialogueAlignmenter.hpp>
#include <Dialogue\IDialogue.hpp>

#include "CDialogueContext.h"
#include "CPlainTextContent.h"
#include "CTickByWord.h"

using namespace CEGUI;
namespace xihad { namespace dialogue
{
	DialogueParser::DialogueParser()
	{
		
	}

	void DialogueParser::initialise()
	{
		SchemeManager& schemeMgr = SchemeManager::getSingleton();
		if (!schemeMgr.isDefined("Generic")) schemeMgr.createFromFile("Generic.scheme");
		if (!schemeMgr.isDefined("Xihad")) schemeMgr.createFromFile("Xihad.scheme");

		//
		Window* root = WindowManager::getSingleton().loadLayoutFromFile("MySample.layout");
		GUIContext& context = System::getSingleton().getDefaultGUIContext();
		context.setDefaultFont("simhei-14");
		//

		context.setRootWindow(root);
		mBaseWindow = root->createChild("Xihad/Frame");
		mBaseWindow->setProperty("Position", "{{0, 0}, {0.5, 0}}");
		mBaseWindow->setProperty("Size", "{{1, 0}, {0.5, 0}}");
		
		CDialogueContext* factory = new CDialogueContext(*mBaseWindow);
		CDialogueAlignmenter* alignmenter = new CDialogueAlignmenter(400);
		mBuilder = new CAlignedDialogueBuilder(factory, alignmenter);

		factory->drop();
		alignmenter->drop();
	}

	IDialogue* DialogueParser::generateDialogue()
	{
		const Font& font  = *mBaseWindow->getFont();
		ITextContent* content = new CPlainTextContent(font,(utf8*)
			"接下来我们定义重力矢量。是的，你可以使重力朝向侧面（或者你只好转动你的显示器）。."
			"并且，我们告诉世界当物体停止移动时允许物体休眠。一个休眠中的物体不需要任何模拟.");
		mBuilder->addText(content);
		content->drop();

		mBuilder->newParagraph();
		content = new CPlainTextContent(font, (utf8*)
			"并且，我们告诉世界当物体停止移动时允许物体休眠。一个休眠中的物体不需要任何模拟."
			);
		mBuilder->addText(content);
		content->drop();

		IDialogue* dialog = mBuilder->build();
		ITickMethod* tickMethod = new CTickByWord(*dialog, 0.2f);
		dialog->setTickMethod(tickMethod);
		tickMethod->drop();

		return dialog;
	}

	DialogueParser::~DialogueParser()
	{
		delete mBuilder;
	}

}}