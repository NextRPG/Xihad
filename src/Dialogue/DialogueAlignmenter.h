#pragma once

namespace xihad { namespace dialogue 
{
	class DialogueAlignmenter
	{
	public:
		explicit DialogueAlignmenter();	
		
		void nextLine();
		void splitSection();
		
		void getCurrentLineHeight();
		void getTotalHeight();
		void setLineSpacing();
		
		void advance();
	};
}}

