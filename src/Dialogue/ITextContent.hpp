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
		
		/// 获取指定位置所属的字符区间
		/**
		 * @param [in, out] bgnIndex
		 * @param [out] endIndex
		 */
		virtual void getLetterRange(unsigned& bgnIndex, unsigned* endIndex = 0) const = 0;

		/// 获取指定位置所属的单词区间
		virtual void getWordRange(unsigned& bgnIndex, unsigned* endIndex = 0) const = 0;

		/// Split text into two parts [0, index) and [index, end), both parts' len > 0
		/**
		 * @param index The letter contains the specified index will be the end of left part (i.e, this)
		 * @return splitted text. If index is not valid, then return nullptr. valid interval is (0, end)
		 */
		virtual ITextContent* split(unsigned index) = 0;

		/// Get end index of this text
		virtual unsigned endIndex() const = 0;

		/// Decide whether this text is empty
		virtual bool empty() const = 0;

		/// Compute the result of filling this text into area that limitted with the specified width
		virtual SFillResult fillHorizontal(unsigned widthLimit, bool allowEmpty = true) const = 0;
	};
}}

