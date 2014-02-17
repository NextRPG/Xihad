#pragma once
#include <vector>
#include <list>
#include "STextSection.hpp"

namespace xihad { namespace dialogue 
{
	class ITickMethod;
	class CDialogue 
	{
	public:
		void setTickMethod(ITickMethod* method);
		bool atEnd();
		bool atBegin();

	private:
		typedef std::vector<STextSection> Sections;
		Sections mSections;

		struct TickEvent 
		{
			int sectionIndex;
			std::wstring event;
		};
		std::list<TickEvent> mEvents;

		// 记录当前显示位置
		Sections::iterator	mCurrentSection;
		std::wstring::iterator	mCurrentChar;

		ITickMethod* mTickMethod;
	};



}}

