#pragma once
#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include "InheritancePath.h"

namespace xihad { namespace ngn
{
	/// 继承树
	/**
	 * 可以动态注册类型的树层次结构。类与类之间可以有单继承关系，虽然不知道多继承，但是完全可以利用
	 * 现有机制来达成同样的效果。
	 * 
	 * @author etnlGD
	 * @date 2013年12月11日 21:33:33
	 */
	class InheritanceTree
	{
	public:
		/// 注册新类型的结果
		/**
		 * @see registerType
		 */
		enum RegisterResult
		{
			SUCCESS,				///< 注册成功
			CLASS_REDEFINE,			///< 类型名已经被占用
			CLASS_NAME_INVALID,		///< 类型名无效
			NO_BASE_CLASS_DEFINE,	///< 指定基类不存在
		};

		typedef std::vector<InheritancePath::Node>::const_iterator Iterator;

	public:
		InheritanceTree();

		~InheritanceTree();

		/// 向继承树中注册新的类型
		/**
		 * @param typeName 指定新类型的名称，不能为空字符串
		 * @param baseClass 指定新类型的基类名称，默认值为空字符串，表示无基类。
		 * @see RegisterResult
		 */
		RegisterResult registerType(const std::string& typeName, const std::string& baseClass = "");
		
		/// 取得指定类型的继承关系链
		/**
		 * @see InheritanceChain
		 */
		InheritancePath getInheritanceChain(const std::string& className) const;

		/// 判断是否包含制定类型
		bool containsType(const std::string& className) const;

		/// 取得指向第一个注册类型的迭代器
		Iterator begin() const;

		/// 取得指向已注册类型的结尾的迭代器
		Iterator end() const;

	private:
		enum {
			INVALID_BASE_INDEX = -1, ///< 无基类时使用的索引
		};

		struct impl;
		boost::scoped_ptr<impl> mImpl;
	};
}}

