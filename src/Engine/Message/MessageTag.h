#pragma once
#include <string>
#include <list>

namespace xihad { namespace ngn 
{
	/************************************************************************/
	/* 
	/************************************************************************/
	class MessageTag
	{
	public:
		typedef std::string TagType;
		typedef std::list<std::string>::const_iterator const_itr;

		MessageTag(const std::string& pParseSource = "", char pSeparator = '.');

		const_itr begin() const;

		const_itr end() const;

		bool empty() const;

		bool operator==(const MessageTag& pOther) const;

		bool operator!=(const MessageTag& pOther) const
		{
			return !((*this) == pOther);
		}

		bool operator<(const MessageTag& pOther) const;

		const std::string& toString() const;

		char getSeparator() const 
		{
			return mSeparator;
		}

	private:
		char mSeparator;
		std::string mOrginTag;
		std::list<std::string> mTags;
	};
	
	std::ostream& operator<<(std::ostream& out, const MessageTag& pTag);
}}