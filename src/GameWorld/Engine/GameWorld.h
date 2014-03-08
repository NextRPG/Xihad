#pragma once
#include <boost\scoped_ptr.hpp>

namespace xihad { namespace ngn
{
	struct GameWorldImpl;
	class UpdateHandler;
	class GameScene;

	/// 维护游戏世界的时间线来控制游戏场景与逻辑的更新
	class GameWorld
	{
	public:
		explicit GameWorld(float defaultStepSize = 1.0f/60);

		virtual ~GameWorld();

		GameScene* setScene(GameScene* scene);
		
		GameScene* getScene() const;

		virtual void start();

		virtual void update(float dtSeconds);

		virtual void stop();

		/// 设置单次步进的时间间隔
		virtual void setSingleStepSeconds(float secs);
		
		/// 取得单次步进的时间间隔
		virtual float getSingleStepSeconds() const;

		/// 设置暂停状态
		/**
		 * 暂停之后，才可以对时间线步进，否则无效
		 * @see singleStep()
		 */
		virtual void setPaused(bool puase);

		/// 判断是否暂停
		/**
		 * @see singleStep()
		 */
		virtual bool isPaused() const;

		/// 设置时间线缩放
		/**
		 * 默认为1.0，设置事件缩放比例后，每当世界时间经过 secs ，那么虚拟世界时间会经过 scale*secs 秒。
		 * 此值不会影响 singleStep 过程，即 singleStep 过程总是以 singleStepSeconds 更新世界。
		 */
		virtual void setTimeScale(float scale);

		/// 取得时间缩放比例
		virtual float getTimeScale() const;
		
		/// 步进时间线
		/**
		 * 相当于调用 update(getSingleStepSeconds()) ，必须在暂停状态下才有意义
		 * @see isPaused() getSingleStepSeconds()
		 */
		virtual void singleStep();

	private:
		struct impl;
		boost::scoped_ptr<impl> mImpl;
	};
}}