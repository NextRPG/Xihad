#pragma once
#include <string>
#include <irrlicht/IReferenceCounted.h>

namespace xihad { namespace dialogue 
{
	class ITextContent : public virtual irr::IReferenceCounted
	{
	public:
		struct SFillResult
		{
			int width;
			int height;
			unsigned splitIndex;
		};

	public:	
		virtual ~ITextContent() {}
		
		virtual std::wstring nextLetter(
			unsigned bgnIndex, unsigned* endIndex = nullptr) const = 0;
		
		virtual std::wstring nextWord(
			unsigned bgnIndex, unsigned* endIndex = nullptr) const = 0;

		/// Split text into two parts [0, index) and [index, end)
		/**
		 * @param index The end of left part (i.e, this)
		 * @return splitted text. If index is not valid, then return nullptr.
		 */
		virtual ITextContent* split(unsigned index) = 0;

		virtual unsigned endIndex() const = 0;

		virtual bool empty() const = 0;

		virtual SFillResult fillHorizontal(unsigned widthLimit, bool allowEmpty = true) const = 0;
	};
}}

