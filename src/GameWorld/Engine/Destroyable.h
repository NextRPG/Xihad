#pragma once

namespace xihad { namespace ngn
{
	class Destroyer;
	class Destroyable
	{
	protected:
		/// 受保护的析构函数，外界必须通过destroy()方法调用
		/**
		 * @see #destroy()
		 */
		virtual ~Destroyable() {}

		friend class Destroyer;

		/// 设置生命期监听者
		/**
		 * @param lifeManager 监听者，当对象被摧毁的时候，会向此生命期监听者通知摧毁事件
		 */
		virtual void setDestroyer(Destroyer* lifeManager) = 0;

	public:
		/// 摧毁对象，如果成功则会调用析构函数
		virtual bool destroy() = 0;

		/// 返回父节点设置的生命期监听者
		/**
		 * @see setLifeManager()
		 * @return binding manager
		 */
		virtual Destroyer* getDestroyer() const = 0;
	};
}}

