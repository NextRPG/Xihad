#pragma once
#include "boost\scoped_ptr.hpp"
#include "boost\shared_ptr.hpp"
#include "irr_ptr.h"

namespace irr
{
	class IrrlichtDevice;
}
namespace xihad { namespace ngn
{
	struct GameEngineImpl;
	class GameWorld;

	/// 游戏引擎启动器
	/**
	 * TODO: 
	 *	1. 处理I/O事件，处理窗口事件
	 *	2. 初始化文件系统
	 *	3. 初始化场景，场景组件系统，添加游戏对象
	 *	
	 *	@author etnlGD
	 *	@date 2013年12月14日 21:25:06
	 */
	class GameEngine
	{
	public:
		enum LoopStatus
		{
			INITIALIZED, RUNNING, STOPPED
		};

	public:
		explicit GameEngine(int frameCount = 60);
		explicit GameEngine(GameWorld& world, int frameCount = 60);
		virtual ~GameEngine();

		boost::shared_ptr<GameWorld> getWorld();

		irr_ptr<irr::IrrlichtDevice> getDevice();

		// if not getDevice().empty(), then just return false
		bool initDevice(irr_ptr<irr::IrrlichtDevice> device);

		virtual bool isRunning() const;

		/// Enter main game loop
		/** 
		 * 游戏退出条件：窗口关闭或 #stop() 被调用
		 * @see stop()
		 * @return false if this has already been started
		 */
		virtual bool launch();
		
		/// Stop game loop if the loop has been started
		/** 
		 * @return if enter #STOPPED status
		 */
		virtual bool stop();

		/// 设置断点临界时间
		/**
		 * 如果单次循环的时间超过这个临界值，那么被视为触发了某个断点。下次
		 */
		void setBreakPointThresholdTime(float thresholdSeconds);

		/// 取得断点临界时间
		/**
		 * @see setBreakPointThresholdTime()
		 */
		float breakPointThresholdTime() const;

		/// 设置是否让进程提前完成单次循环后进入睡眠，等待下一次循环
		void setNeverSleep(bool neverSleep);

		/// 判断是否让进程提前完成单次循环后进入睡眠，等待下一次循环
		bool isNeverSleep() const;

		/// 设置是否显示帧率
		void setShowFPS(bool showFPS);

		/// 判断是否显示帧率
		bool isShowFPS() const;

	protected:
		void afterWorldStep( float stepTime );

	private:
		void initWithWorld(GameWorld* world, float frameInterval);
		
	private:
		struct impl;
		boost::scoped_ptr<impl> mImpl;
	};
}}

