#pragma once
#include "CppBase/xassert.h"
#include "Destroyable.h"

namespace xihad { namespace ngn
{
	class UpdateStatusChangeListener;
	class Destroyer;
	class Timeline;

	/// 游戏主循环更新对象
	/** 
	 * 在游戏运行过程中，引擎会保证在每一次迭代中，对所有注册的更新对象进行更新调用。
	 * <p>生命周期：
	 *	- <pre>
	 *		-(constructor)-> #BORN -(#STARTING)-> #UPDATED +(#UPDATING)-> #UPDATED
	 *	  												   +(#STOPPING)-> #DEAD -(#DESTROYING)-> destructed
	 *	  </pre>
	 *	- 中间状态时， #intermidiateStatus() return true
	 *	- 中间状态时，任何函数调用不会改变对象状态
	 *	- #STARTING, #UPDATING 状态时，调用 #stop() 方法，是有效的
	 *	- 除此之外，任何中间状态下的命令都会被丢弃掉
	 *	
	 * <p>游戏更新对象严格遵循层次结构
	 *	- 除了根节点，每一个对象节点都有一个父节点
	 *	- 如果父节点处于 #BORN, #UPDATED, #DEAD 状态，那么子节点也同样会处于相同状态
	 *	- 如果子节点处于 #UPDATING, #STOPPING, #DESTROYING 等中间状态，那么父节点肯定处于相同状态
	 *	- 如果子节点处于 #STARTING 状态，可以保证父节点不处于 #BORN, #DEAD, #DESTROYING 状态
	 *	
	 *	@author etnlGD
	 *	@date 2013年12月9日 20:28:41
	 */
	class UpdateHandler : public Destroyable
	{
	public:

		/// 更新对象的生命期
		/**
		 * 顺序是不能改变的，因此你可以做如下测试：
		 * <p> if (BORN < status < DEAD) then doSomething() end
		 */
		enum Status
		{
			BORN,		///< 构造函数调用后所处状态
			STARTING,	///< 第一次调用 #start() 方法中
			UPDATING,	///< #update() 调用中
			UPDATED,	///< 等待下一次更新
			STOPPING,	///< #stop() 调用中
			DEAD,		///< #stop() 返回后
			DESTROYING	///< #destroy() 调用中
		};

	protected:
		virtual ~UpdateHandler() {}

	public:

		/// 开始进入游戏更新
		/**
		 * 函数返回后，一般会进入 #UPDATED 状态，但是如果期间调用了 #stop() 方法，则会进入 #DEAD 状态。
		 * 如果是第一次调用此函数，那么调用过程中会处于 #STARTING 状态
		 * @return true 如果这是一次有效的开始命令
		 */
		virtual bool start() = 0;

		/// 更新对象
		/**
		 * 在游戏主循环中，保证会被调用。如果期间没有调用 #stop() 则恢复到 #UPDATED 状态
		 * @param time 当前场景时间
		 * @return true 如果有效得进行了更新
		 */
		virtual bool update(const Timeline& time) = 0;

		/// 停止对象更新
		/**
		 * 此方法调用之后，对象不能在进行 #update(), #start(). 但是你仍然可以向对象发出其他命令，
		 * 比如: #status(). 或者其他子类实现的方法。
		 * <p> 方法返回后，对象处于 #DEAD 状态
		 * 
		 * @return true 如果有效得停止了自身
		 */
		virtual bool stop() = 0;

		/// 摧毁更新对象
		/**
		 * 如果对象不处于 #BORN 状态，那么此方法保证会在 #~UpdateHandler() 方法调用之前调用
		 * #stop() 方法，让对象处于 #DEAD 状态。
		 * <p>方法返回 true 之后，不允许再对该指针进行任何形式引用
		 * 
		 * @return true 如果有效得摧毁了自身
		 */
		virtual bool destroy() = 0;

		/// 返回对象所处状态
		/**
		 * @see Status
		 * @return current status
		 */
		virtual Status status() const = 0;

		/// 是否处于中间状态
		/**
		 * @return true if #status() from { #STARTING, $UPDATING, #STOPPING, #DESTROYING }
		 */
		bool intermediateStatus() const
		{
			Status mStatus = status();
			switch (mStatus)
			{
			case BORN:
			case UPDATED:
			case DEAD:
				return false;
			default:
				return true;
			}
		}
	};

}}

