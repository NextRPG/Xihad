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
#include "CTickByLetter.h"
#include <assert.h>
#include <windows.h>
#include <stringapiset.h>

using namespace CEGUI;
namespace xihad { namespace dialogue
{
	DialogueParser::DialogueParser()
	{
		
	}

	void DialogueParser::initialise(Window& dialog)
	{
		mBaseWindow = dialog.getChild("__auto_container__");

		CDialogueContext* factory = new CDialogueContext(*mBaseWindow);
		CDialogueAlignmenter* alignmenter = new CDialogueAlignmenter((unsigned)mBaseWindow->getPixelSize().d_width);
		alignmenter->setKerningHeight(10);
		mBuilder = new CAlignedDialogueBuilder(factory, alignmenter);

		factory->drop();
		alignmenter->drop();
	}

	IDialogue* DialogueParser::generateDialogue()
	{
		const Font& font  = *mBaseWindow->getFont();
		ITextContent* content = new CPlainTextContent(font,(utf8*)
			"接下来我们定义重力矢量。是的，你可以使重力朝向侧面（或者你只好转动你的显示器）。 "
			"并且，我们告诉世界当物体停止移动时允许物体休眠。一个休眠中的物体不需要任何模拟 ");
		mBuilder->addText(content);
		content->drop();
		
		IDialogue* dialog = mBuilder->build();
		ITickMethod* tickMethod = new CTickByLetter(*dialog, 0.2f);
		dialog->setTickMethod(tickMethod);
		tickMethod->drop();

		return dialog;
	}

	DialogueParser::~DialogueParser()
	{
		delete mBuilder;
	}

}}