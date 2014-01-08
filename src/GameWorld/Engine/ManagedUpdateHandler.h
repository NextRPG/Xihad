#pragma once
#include "UpdateHandler.h"

namespace xihad { namespace ngn
{
	/// UpdateHandler 接口的基础实现
	/**
	 * 此类保证了状态和行为的一致性。例如：当不在 #UPDATED 状态时，对 #update() 接口的调用是无效的。
	 * 一般来说，实现 UpdateHandler 接口的类都应该继承自此抽象基类。
	 */
	class ManagedUpdateHandler : public virtual UpdateHandler
	{
	public:
		ManagedUpdateHandler();
		
		virtual bool start()  override final;
		
		virtual bool update(const Timeline&) override final;
		
		virtual bool stop()   override final;
		
		virtual bool destroy()override final;
		
		virtual Status status() const override final;
		
		virtual Destroyer* getDestroyer() const override final;

		///< 判断对象是否正在摧毁中
		bool isDestroying() const;

	protected:
		virtual ~ManagedUpdateHandler();

		virtual void setDestroyer(Destroyer* m) override final;

		/// 从 #STARTING 状态切换至 #UPDATED 状态时，被回调
		virtual void onStart()  = 0;

		/// 从 #UPDATING 状态切换至 #UPDATED 状态时，被回调
		virtual void onUpdate(const Timeline&) = 0;

		/// 从 #STOPPING 状态切换至 #DEAD 状态时，被回调
		virtual void onStop()   = 0;

		/// 进入析构函数之前调用
		/**
		 * onDestroy() 之后会立即调用析构函数，但是这个方法和析构函数还是有很大区别的（不能直接
		 * 把这个方法的代码复制到析构函数的最前面）。如果对象是子类的实例，调用顺序为：
		 * <pre>
		 *		onDestroy() -> ~ChildDtor() -> ~SelfDtor()
		 * </pre>
		 * 因此，需要严格遵循依赖关系，将被依赖数据放在后面摧毁。比如：某段代码直接或间接得依赖子类
		 * 的成员变量，那么就应该把这段代码放在 onDestroy() 方法中，而不应该在 ~ChildDtor() 之后
		 * （即 ~SelfDtor() 中）。
		 * 
		 * 默认实现为空实现
		 */
		virtual void onDestroy();

	private:
		void fireStatusChanged(Status newStatus);

		void execPendingCommand(Status preStatus);

	private:
		int mStatus;
		Destroyer* mLifeManager;
	};
}}

