#pragma once

namespace xihad { namespace ngn
{
	/// 监听游戏对象标签事件
	template <typename TaggedObject, typename TagT, unsigned MaxTagCount_>
	class TagManager
	{
	public:
		enum { MaxTagCount = MaxTagCount_, InvalidIndex = -1 };
		typedef TaggedObject Object;
		typedef TagT Tag;

	public:
		virtual ~TagManager() {}

		/// 向游戏对象成功添加某个标签时回调
		virtual void onTagAdded(TaggedObject* obj, unsigned tagIndex) = 0;

		/// 从游戏对象成功移除某个标签时回调
		virtual void onTagRemoved(TaggedObject* obj, unsigned tagIndex) = 0;

		virtual unsigned unprojectTag(const Tag& tag, bool addIfDontExists) const = 0;
	};
}}

