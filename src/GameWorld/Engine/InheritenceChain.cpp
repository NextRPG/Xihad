#include "InheritenceChain.h"

using namespace std;
namespace xihad { namespace ngn
{

	InheritenceChain::InheritenceChain() :
		mBegin(-1), hierarchy(nullptr)
	{
	}

	InheritenceChain::InheritenceChain( int beginIdx, const TypeVector& hierarchy ) :
		mBegin(beginIdx), hierarchy(&hierarchy)
	{
		xassert(mBegin >= 0 && this->hierarchy != nullptr);
	}

	const std::string& InheritenceChain::root() const
	{
		if (mBegin == -1 || !hierarchy) 
			throw std::exception("Invalid class. check begin() != end()");

		int rootIdx = (*hierarchy)[mBegin].rootIndex;
		xassert (rootIdx >= 0);
		return (*hierarchy)[rootIdx].typeName;
	}

	InheritenceChain::Iterator InheritenceChain::begin() const
	{
		return Iterator(mBegin, hierarchy);
	}

	InheritenceChain::Iterator InheritenceChain::end() const
	{
		return Iterator(-1, hierarchy);
	}

	InheritenceChain::Iterator::Iterator( int beginIdx, const TypeVector* hierarchy )
		: idx(beginIdx), mTree(hierarchy)
	{
		if (hierarchy == nullptr && beginIdx >= 0)
			throw std::exception("Invalid iterator state");
	}

	bool InheritenceChain::Iterator::operator!=( const Iterator& other )
	{
		if (idx < 0 && other.idx < 0)
			return false;

		return mTree != other.mTree || idx != other.idx;
	}

	InheritenceChain::Iterator& InheritenceChain::Iterator::operator++()
	{
		if (idx < 0)
			throw std::exception("Invalid iterator state");
		
		xassert(mTree != nullptr);
		idx = (*mTree)[idx].baseIndex;
		return *this;
	}

	const std::string& InheritenceChain::Iterator::operator*()
	{
		if (idx < 0)
			throw std::exception("Invalid iterator state");

		xassert(mTree != nullptr);
		return (*mTree)[idx].typeName;
	}

}}

