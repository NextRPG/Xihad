#include "InheritanceTree.h"
#include <unordered_map>

using namespace std;
namespace xihad { namespace ngn
{
	struct InheritanceTree::impl
	{
		vector<InheritancePath::Node>	hierarchy;
		unordered_map<string, int>  type2Index;
	};

	InheritanceTree::InheritanceTree() : mImpl(new impl)
	{
	}

	InheritanceTree::~InheritanceTree()
	{
	}

	InheritanceTree::RegisterResult InheritanceTree::registerType( 
		const string& typeName, const string& baseClass /*= ""*/ )
	{
		if (typeName.empty()) 
			return CLASS_NAME_INVALID;

		if (containsType(typeName)) 
			return CLASS_REDEFINE;

		InheritancePath::Node type;
		type.typeName = typeName;
		if (!baseClass.empty()) 
		{
			auto got = mImpl->type2Index.find(baseClass);
			if (got == mImpl->type2Index.end())
			{
				return NO_BASE_CLASS_DEFINE;
			}
			else
			{
				type.baseIndex = got->second;
				type.rootIndex = mImpl->hierarchy[type.baseIndex].rootIndex;
			}
		}
		else
		{
			type.baseIndex = INVALID_BASE_INDEX;
			type.rootIndex = mImpl->hierarchy.size();
		}

		mImpl->type2Index[typeName] = mImpl->hierarchy.size();
		mImpl->hierarchy.push_back(type);
		return SUCCESS;
	}

	bool InheritanceTree::containsType( const std::string& className ) const
	{
		return mImpl->type2Index.find(className) != mImpl->type2Index.end();
	}

	InheritancePath InheritanceTree::getInheritanceChain( const std::string& className ) const
	{
		auto got = mImpl->type2Index.find(className);

		if (got != mImpl->type2Index.end())
			return InheritancePath(got->second, mImpl->hierarchy);
		else
			return InheritancePath();
	}

	InheritanceTree::Iterator InheritanceTree::begin() const
	{
		return mImpl->hierarchy.begin();
	}

	InheritanceTree::Iterator InheritanceTree::end() const
	{
		return mImpl->hierarchy.end();
	}

}}

