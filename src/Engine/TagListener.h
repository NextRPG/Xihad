#pragma once
#include <string>

namespace xihad { namespace ngn
{
	class GameObject;

	/// 监听游戏对象标签事件
	class TagListener
	{
	public:
		virtual ~TagListener() {}

		/// 向游戏对象成功添加某个标签时回调
		virtual void onTagAdded(GameObject* obj, const std::string& tag) = 0;

		/// 从游戏对象成功移除某个标签时回调
		virtual void onTagRemoved(GameObject* obj, const std::string& tag) = 0;
	};
}}

