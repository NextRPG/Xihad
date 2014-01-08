#pragma once

namespace xihad { namespace ngn
{
	class Destroyable;
	/// 更新对象生命周期监听者
	/**
	 * 当更新对象被摧毁的时候，会回调此接口。一个更新对象同一时刻只能有最多一个生命期监听者
	 */
	class Destroyer
	{
	public:
		virtual ~Destroyer() {}

		/// 回调接口
		/**
		 * 在 toDestroy 被析构前调用
		 * 
		 * @see manageUpdater()
		 * @param toDestroy 指定即将被析构的对象，此对象必须通过 #manageUpdater() 注册过
		 */
		virtual void onChildDestroy(Destroyable* toDestroty) = 0;

		/// 生命期监管者是否正在被摧毁
		/**
		 * UpdateLifeManager 被摧毁时，会摧毁其监管的对象。而子对象被摧毁时，监管者不一定正在被摧毁
		 * @return if the manager is destroying
		 */
		virtual bool isDestroying() = 0;

	protected:
		/// 监管更新对象
		/**
		 * @return the previous life manager of u
		 */
		Destroyer* manageDestroyable(Destroyable* u);
		
		/// 取消对更新对象的监管
		/**
		 * @return the previous life manager of u
		 */
		Destroyer* unmanageDestroyable(Destroyable* u);
	};
}}

