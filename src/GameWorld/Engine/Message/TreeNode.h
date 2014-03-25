#pragma once
#include "boost\scoped_ptr.hpp"

namespace xihad { namespace ngn 
{
	template <typename ValueType>
	class TreeNode	//noncopyable
	{
	public:
		typedef ValueType ValueType;

		TreeNode() : mParent(nullptr), mLeft(nullptr), mRight(nullptr) {}
	
		TreeNode(const ValueType& value,
			TreeNode* parent = nullptr, TreeNode* left = nullptr, TreeNode* right = nullptr) :
				mValue(value), mParent(parent), mLeft(left), mRight(right) {}

		TreeNode* left() const { return mLeft.get(); }
		
		TreeNode* right() const { return mRight.get(); }

		TreeNode* parent() const { return mParent; }

		void setParent(TreeNode* parent) { mParent = parent; } 

		void setLeft(TreeNode* node) { mLeft.reset(node); }
		
		void setRight(TreeNode* node) { mRight.reset(node); }

		ValueType& operator*() { return mValue; }

		ValueType* operator->() { return &mValue; }

	private:
		TreeNode<ValueType>& operator=(const TreeNode<ValueType>& o);

		ValueType mValue;
		TreeNode* mParent;
		boost::scoped_ptr<TreeNode> mLeft;			
		boost::scoped_ptr<TreeNode> mRight;			
	};
}}
