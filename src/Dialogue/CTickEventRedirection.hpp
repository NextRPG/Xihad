#pragma once
#include <cassert>
#include <algorithm>
#include "STickEvent.hpp"

namespace xihad { namespace dialogue
{
	template <typename Container>
	class CTickEventRedirection
	{
		typedef typename Container::iterator iterator;

	public:
		CTickEventRedirection(Container& events) :
			mEndIter(events.end()), mAddedSectionCount(0), mVisitedSectionCount(0)
		{
			// Only update tick events whose section index > 0
			mEventIter = std::find_if(events.begin(), mEndIter, 
				[](STickEvent& tick) -> bool {
					return tick.sectionIndex > 0;
			});
		}

		void onInsertion(int insertCount)
		{
			mAddedSectionCount += (insertCount-1);
			++mVisitedSectionCount;

			assert(mVisitedSectionCount > 0);
			while (mEventIter != mEndIter)
			{
				assert(mEventIter->sectionIndex >= mVisitedSectionCount);

				if (mEventIter->sectionIndex == mVisitedSectionCount)
				{
					mEventIter->sectionIndex += mAddedSectionCount;
					++mEventIter;
				}
				else
				{
					break;
				}
			}

		}

	private:
		iterator mEventIter;
		iterator mEndIter;
		unsigned mAddedSectionCount;
		unsigned mVisitedSectionCount;
	};
}}

