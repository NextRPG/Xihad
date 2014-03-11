#pragma once
#include "boost\scoped_ptr.hpp"

namespace xihad { namespace ngn
{
	class GameWorld;
	class NativeWindow;
	class WindowRenderer;
	class FrameObserver;

	class GameEngine
	{
	public:
		enum LoopStatus
		{
			INITIALIZED, RUNNING, STOPPED
		};

	public:
		explicit GameEngine(NativeWindow& wnd, GameWorld* world = 0, float defaultFrameTime = 1.f/60);

		virtual ~GameEngine();

		GameWorld* getWorld();

		NativeWindow* getWindow();

		/// add frame observer with specified order
		/**
		 * You can set the observer's order explicitly.
		 * The larger the order is, the later the observer will be notified.
		 * 
		 * Notice: 
		 * 1. Multiple observers can share a same order, notify order between them are not specified
		 * 2. If add a observer which has already been added, then the latter one order will be effective.
		 *	  You can depend on this to dynamically change a observer's order.
		 */
		virtual void addFrameObserver(FrameObserver& observer, int order = 0);

		virtual void removeFrameObserver(FrameObserver& observer);

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

		virtual void setFrameTime(float);

	private:
		WindowRenderer* getRenderer();

		float fireFrameBegin();

		void fireFrameEnd(float bgnTime);
		
	private:
		struct impl;
		boost::scoped_ptr<impl> mImpl;
	};
}}

