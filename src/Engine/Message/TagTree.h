#pragma once
#include "TreeNode.h"
#include "MessageTag.h"
#include "CppBase/xassert.h"

#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <boost/call_traits.hpp>

namespace xihad { namespace ngn 
{
	class BadDereference : public std::runtime_error
	{
	public:
		BadDereference() : std::runtime_error("can not deference.") {}
	};

	template<typename ValueType> class TagTree;

	/// 深度优先遍历以某节点为父节点的子树的 Iterator
	/**
	 * 由 TagTree 衍生出来的深度优先遍历器，可以以 TagTree 中的任意节点作为根节点进行遍历，只能前向
	 * 遍历，与 STL 的遍历器不兼容
	 * 
	 * @see TagTree
	 * @author Alpha
	 */
	template<typename ValueType>
	class SubtreeIterator
	{
	public:
		typedef TagTree<ValueType> Tree;
		typedef SubtreeIterator self;

		/**
		 * @see TagTree
		 * @param tree 衍生该遍历器的 TagTree
		 * @param root 以该节点作为根节点开始遍历子节点
		 */
		explicit SubtreeIterator(Tree* tree = nullptr, typename Tree::Node* root = nullptr)
			: mTree(tree)
		{
			if (root) mStack.push(root);
		}

		/// 前置递增
		/**
		 * 在递增操作前应该先调用 #hasNext(), 如果 #hasNext() 返回 false，调用该函数不
		 * 做处理
		 * @return 返回前进之后的自身的引用
		 */
		self& operator++()
		{
			if (mStack.empty()) return *this;

			auto current = mStack.top();
			mStack.pop();
			if (current->right()) mStack.push(current->right());
			if (current->left()) mStack.push(current->left());
			return *this;
		}

		///后置递增
		/**
		 * @see #operator++()
		 */
		self operator++(int) 
		{
			self last(*this);
			++(*this);
			return last;
		}
		
		/// 判断是否还有节点没有遍历
		/**
		 * @return	如果到达尾部，返回假，否则为真
		 */
		bool hasNext() const
		{
			return !mStack.empty();
		}

		/**
		 * @see TagTree::ReturnType
		 * @return 返回当前节点对应的监听者链表
		 */
		typename Tree::ReturnType operator*()
		{
			if (mStack.empty()) 
				throw BadDereference(); 
			
			return mTree->getList(mStack.top());
		}

	private:
		std::stack<typename Tree::Node*> mStack;
		Tree* mTree;
	};

	/// 遍历从某个标签开始的所有上层标签对应的节点
	/**
	 * 如果存在标签 a, a.b, a.b.c, 且由 a.b.c 对应的节点创建， 则依次遍历 a.b.c, a.b, a
	 * @see TagTree
	 * @author Alpha
	 */
	template<typename ValueType>
	class PathIterator
	{
	public:
		typedef TagTree<ValueType> Tree;
		typedef PathIterator<ValueType> self;

		/**
		 * @see TagTree
		 * @param tree 衍生该遍历器的 TagTree
		 * @param lastNode 以该节点开始逐层向上遍历
		 */
		explicit PathIterator(Tree* tree = nullptr, typename Tree::Node* lastNode = nullptr) 
			: mTree(tree), mCursor(lastNode)
		{

		}

		/// 前置递增
		/**
		 * 在递增操作前应该先调用 #hasNext(), 如果 #hasNext() 返回 false，调用该函数不
		 * 做处理
		 * @return 返回前进之后的自身的引用
		 */
		self& operator++()
		{
			if (mCursor)
				mCursor = mCursor->parent();
			return *this;
		}

		///后置递增
		/**
		 * @see #operator++()
		 */
		self operator++(int) 
		{
			self last(*this);
			++(*this);
			return last;
		}

		/// 判断是否还有节点没有遍历
		/**
		 * @return	如果到达尾部，返回假，否则为真
		 */
		bool hasNext() const
		{
			return mCursor != nullptr;
		}

		/**
		 * @see TagTree::ReturnType
		 * @return 返回当前节点对应的监听者链表
		 */
		typename Tree::ReturnType operator*()
		{
			if (mCursor == nullptr) 
				throw BadDereference();

			return mTree->getList(mCursor);
		}

	private:
		Tree* mTree;		
		typename Tree::Node* mCursor;
	};

	/// 保存了[标签，监听者链表]容器，结构上是二叉树
	/**
	 * 树的组织形式：
	 * <pre>
	 *			NULL Head
	 *				\
	 *				(a)
	 *			/		\
	 *		(a.b)		(d)
	 *		/	\		/	\
	 * (a.b.c)(a.e)	(d.f)	(g)
	 *	
	 *	左节点代表：标签的子分类，如 a.b 是 a 的左节点
	 *	右节点代表：同级的标签，如 a.b 和 a.e 是同级节点，后插入的会成为先插入的右节点
	 *	实际节点保存的只是标签的最后一个子标签，如 a.b.c 节点保存的只是 c
	 *	每个节点又对应一条链表，链表保存的则是 ValueType
	 *	</pre>
	 *	
	 *	@author alpha
	 */
	template<typename ValueType>
	class TagTree	//noncopyable
	{

	public:
		typedef typename boost::call_traits<ValueType>::param_type ParamType;
		typedef typename ValueType ValueType;

		/// 树的节点
		/**
		 * 保存的值是 MessageTag::TagType
		 * @see MessageTag::TagType
		 * @see TreeNode
		 */
		typedef TreeNode<MessageTag::TagType> Node;

		typedef SubtreeIterator<ValueType> SubtreeIterator;	
		typedef PathIterator<ValueType> PathIterator;

		typedef std::unordered_map<Node*, std::list<ValueType>> NodeMap;
		typedef typename NodeMap::mapped_type& ReturnType;

		/// 遍历整棵树
		/**
		 * @see SubtreeIterator
		 */
		SubtreeIterator begin() 
		{
			return SubtreeIterator(this, mHead.right());
		}
		
		/// 插入节点
		/**
		 *
		 * @see PathIterator
		 * @param messageTag 需要插入的 ValueType 对应的标签
		 * @param param ValueType 的参数形式
		 * @return	如果 messageTag 为空，则返回一个到达尾部的 PathIterator ，
		 *			否则返回从插入的节点开始的 PathIterator
		 */
		PathIterator insert(const MessageTag& messageTag, ParamType param)
		{
			if (messageTag.empty())
			{
				return PathIterator(this);
			}
			
			Node* inserted = insertImpl(messageTag);
			mNodeMap[inserted].push_back(param);
			return PathIterator(this, inserted);
		}

		/// 查找某个标签对应的节点，返回能够遍历子树的遍历器
		/**
		 * @return	返回以找到的节点开始的 SubtreeIterator ，如果 messageTag 为空或者
		 *			没有找到对应的节点，则返回一个到达尾部的 SubtreeIterator 
		 */
		SubtreeIterator findSubtree(const MessageTag& messageTag)
		{
			if (messageTag.empty())
			{
				return SubtreeIterator(this);
			}
			
			Node* found = findImpl(messageTag);
			return SubtreeIterator(this, found);
		}

		/// 查找某个标签对应的节点，返回能够遍历父标签节点的遍历器
		/**
		 * @see PathIterator
		 * @return	如果 messageTag 为空或者没有找到对应的节点，则返回一个到达尾部的 PathIterator ，
		 *			否则返回从找到的节点开始的 PathIterator
		 */
		PathIterator findPath(const MessageTag& messageTag)
		{
			if (messageTag.empty())
			{
				return PathIterator(this, nullptr);
			}

			Node* found = findPathImpl(messageTag);
			return PathIterator(this, found);
		}

		void clear()
		{
			mNodeMap.clear();
			mHead.setParent(nullptr);
			mHead.setRight(nullptr);
			mHead.setLeft(nullptr);
		}

	private:
		Node* addRightNode(const MessageTag::TagType& tag, Node* buddy)
		{
			Node* node = new Node(tag, buddy->parent());
			buddy->setRight(node);
			mNodeMap.insert(NodeMap::value_type(node, NodeMap::mapped_type()));
			return buddy->right();
		}

		Node* addLeftNode(const MessageTag::TagType&tag, Node* parent)
		{
			Node* node = new Node(tag, parent);
			parent->setLeft(node);
			mNodeMap.insert(NodeMap::value_type(node, NodeMap::mapped_type()));
			return parent->left();
		}

		Node* insertImpl(const MessageTag& messageTag)
		{
			Node* current = &mHead;
			auto itr = messageTag.begin();
			auto endCursor = messageTag.end();

			do
			{
				bool same = *(*current) == (*itr);
				if (stepOutDetection<ValueType>(same, itr, current, messageTag))
				{
					if (!same)
					{
						current = addRightNode(*itr, current);
						++itr;
					}
					break;
				}
			} while (itr != endCursor);

			while (itr != endCursor)
			{
				current = addLeftNode(*itr, current);
				++itr;
			}

			return current;
		}

		Node* findImpl(const MessageTag& messageTag)
		{
			Node* current = &mHead;
			auto itr = messageTag.begin();

			do 
			{
				bool same = *(*current) == (*itr);			// will excute at least once
				if (stepOutDetection<ValueType>(same, itr, current, messageTag))
				{
					if (same && itr == messageTag.end()) 
						return current;
					break;
				}
			} while (itr != messageTag.end());

			return nullptr;
		}

		Node* findPathImpl(const MessageTag& messageTag)
		{
			Node* current = &mHead;
			auto itr = messageTag.begin();

			do 
			{
				bool same = *(*current) == (*itr);			// will excute at least once

				if ((!same && !current->right()))
				{
					return current->parent();
				}
				
				if (same && (++itr == messageTag.end() || !current->left()))
				{
					return current;
				}

				if (!same) current = current->right(); 
				if (same) current = current->left();
			} while (itr != messageTag.end());

			return nullptr;
		}


		ReturnType getList(Node* node)
		{
			auto itr = mNodeMap.find(node);
			if (itr != mNodeMap.end())
			{
				return ReturnType((*itr).second);
			}

			static NodeMap::mapped_type emptyList;
			static ReturnType nullValue(emptyList);
			return nullValue;
		}

		friend class SubtreeIterator;
		friend class PathIterator;
	private:
		NodeMap mNodeMap;
		Node mHead;							// a null head.
	};

	template<typename ValueType>
	static bool stepOutDetection(bool same, MessageTag::const_itr& itr, 
		typename TagTree<ValueType>::Node*& current, const MessageTag& messageTag)
	{
		if ((!same && !current->right()) ||
			(same && (++itr == messageTag.end() || !current->left())))
		{
			return true;
		}

		if (!same) current = current->right(); 
		if (same) current = current->left();

		return false;
	}
}}
