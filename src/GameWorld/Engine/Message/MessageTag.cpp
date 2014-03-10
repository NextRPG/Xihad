#include "MessageTag.h"
#include <algorithm>
#include <iostream>
#include "CppBase\CStringSplitter.hpp"

namespace xihad { namespace ngn 
{
	MessageTag::MessageTag( const std::string& pParseSource, char pSeparator )
		: mOrginTag(pParseSource), mSeparator(pSeparator)
	{
		CStringSplitter<char> splitter(pSeparator, '%');
		splitter.split(mTags, pParseSource);
	}

	MessageTag::const_itr MessageTag::begin() const
	{
		return mTags.begin();
	}

	MessageTag::const_itr MessageTag::end() const
	{
		return mTags.end();
	}

	bool MessageTag::empty() const
	{
		return mTags.empty();
	}

	bool MessageTag::operator==( const MessageTag& pOther ) const
	{
		return mOrginTag == pOther.mOrginTag;
	}

	const std::string& MessageTag::toString() const
	{
		return mOrginTag;
	}

	bool MessageTag::operator<( const MessageTag& pOther ) const
	{
		return mOrginTag < pOther.mOrginTag;
	}

	std::ostream& operator<<( std::ostream& out, const MessageTag& pTag )
	{
		return out << pTag.toString();
	}

}}