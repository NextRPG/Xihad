#include "InheritancePath.h"

using namespace std;
namespace xihad { namespace ngn
{

	InheritancePath::InheritancePath() :
		mBegin(-1), hierarchy(nullptr)
	{
	}

	InheritancePath::InheritancePath( int beginIdx, const TypeVector& hierarchy ) :
		mBegin(beginIdx), hierarchy(&hierarchy)
	{
		xassert(mBegin >= 0 && this->hierarchy != nullptr);
	}

	const std::string& InheritancePath::root() const
	{
		if (mBegin == -1 || !hierarchy) 
			throw std::exception("Invalid class. check begin() != end()");

		int rootIdx = (*hierarchy)[mBegin].rootIndex;
		xassert (rootIdx >= 0);
		return (*hierarchy)[rootIdx].typeName;
	}

	InheritancePath::Iterator InheritancePath::begin() const
	{
		return Iterator(mBegin, hierarchy);
	}

	InheritancePath::Iterator InheritancePath::end() const
	{
		return Iterator(-1, hierarchy);
	}

	InheritancePath::Iterator::Iterator( int beginIdx, const TypeVector* hierarchy )
		: idx(beginIdx), mTree(hierarchy)
	{
		if (hierarchy == nullptr && beginIdx >= 0)
			throw std::exception("Invalid iterator state");
	}

	bool InheritancePath::Iterator::operator!=( const Iterator& other )
	{
		if (idx < 0 && other.idx < 0)
			return false;

		return mTree != other.mTree || idx != other.idx;
	}

	InheritancePath::Iterator& InheritancePath::Iterator::operator++()
	{
		if (idx < 0)
			throw std::exception("Invalid iterator state");
		
		xassert(mTree != nullptr);
		idx = (*mTree)[idx].baseIndex;
		return *this;
	}

	const std::string& InheritancePath::Iterator::operator*()
	{
		if (idx < 0)
			throw std::exception("Invalid iterator state");

		xassert(mTree != nullptr);
		return (*mTree)[idx].typeName;
	}

}}

