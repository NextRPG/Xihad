#pragma once
#include <vector>
#include "CppBase/xassert.h"

namespace xihad { namespace ngn
{
	/// 继承关系链
	/**
	 * 可以从中取得某个类的所有基类，示例如下：
	 * <pre>
	 *		for (const InheritenceChain::Node& n : chain) dosomething();
	 * </pre>
	 * 
	 * @author etnlGD
	 * @date 2013年12月12日 14:13:02
	 */
	class InheritenceChain
	{
	public:
		struct Node
		{
		public:
			std::string typeName;
			int baseIndex;
			int rootIndex;
		};

		typedef std::vector<Node> TypeVector;

		struct Iterator
		{
		public:
			Iterator(int beginIdx, const TypeVector* hierarchy);

			bool operator!=(const Iterator& other);
			
			Iterator& operator++();
			
			Iterator operator++(int) const
			{
				Iterator cpy = *this;
				return ++cpy;
			}

			const std::string& operator*();

		private:
			int idx;
			const TypeVector* mTree;
		};

	public:

		/**
		 * 用于构造不存在的继承链，初始状态下： begin() == end()
		 */
		InheritenceChain();

		/**
		 * @param beginIdx 指定继承链中叶节点的索引
		 * @param hierarchy 指定记录了所有继承关系容器
		 */
		InheritenceChain(int beginIdx, const TypeVector& hierarchy);

		/// 取得继承链中的顶层类名称
		const std::string& root() const;
		
		/// 取得指向继承链中的第一个条目的迭代器
		Iterator begin() const;

		/// 取得继承链中的终止迭代器
		Iterator end() const;

	private:
		int mBegin;
		const TypeVector* hierarchy;
	};

}}

