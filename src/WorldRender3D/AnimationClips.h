#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <string>

namespace xihad { namespace render3d
{
	struct AnimationClipData;
	class AnimatedMeshComponent;

	/// 动画剪辑集合
	/**
	 * 存储一组动画剪辑，每个动画剪辑由名称、起止帧数和播放速度组成
	 * 
	 * @see ClipData
	 * @author etnlGD
	 * @date 2013年12月15日 02:22:32
	 */
	class AnimationClips
	{
	public:
		/// 从 ptree 中读取 AnimationClip
		/**
		 * ptree 中数据必须为数组，数组中的每个元素代表一个剪辑。
		 * <p>每个剪辑的数据依然是一个数组，其中数组第一个元素为剪辑名称，第二个元素为剪辑的
		 * 起止帧数，第三个元素为剪辑默认播放速度（可选）。
		 * <p>起止帧数同样得是数组，如果这个数组有两个元素，那么第一个元素表示开始帧数，第二
		 * 个元素表示终止帧数；如果只有一个元素，那么这个元素是这个剪辑的终止帧数，而开始帧数
		 * 为上一个剪辑的终止帧数加1。
		 * 
		 * <p>另外，如果 ptree 数组的第一个元素的剪辑名为 default ，那么第三个元素（如果没有
		 * 那么就是第二个元素）为全局的剪辑默认播放速度，一旦后续的剪辑没有第三个元素（单个剪辑的
		 * 默认播放速度），那么就使用全局的剪辑默认播放速度作为该剪辑的默认播放速度
		 * 
		 * @param data 满足数据格式要求的 ptree
		 */
		static AnimationClips loadFromPtree(const boost::property_tree::ptree& data);

		/// 取得默认的动画播放速度
		static float getDefaultAnimationSpeed();

		/// 设置默认的动画播放速度
		/**
		 * 不可以为 0
		 * @param speed 单位为 帧每秒
		 * @return getDefaultAnimationSpeed()
		 */
		static float setDefaultAnimationSpeed(float speed);

	public:
		/// 剪辑集合访问者
		/**
		 * @see traverse()
		 */
		class Visitor
		{
		public:
			virtual ~Visitor() {}

			/// 处理当前剪辑
			/**
			 * @param clipname 剪辑名称
			 * @param clip 剪辑数据
			 */
			virtual void onClip(const std::string& clipname, const AnimationClipData& clip) = 0;
		};

	public:
		/// 构造空的剪辑集合
		AnimationClips();

		/// 让访问者遍历所有剪辑
		/**
		 * 针对每一个剪辑，对访问者调用 Visitor::onClip() 方法
		 */
		void traverse(Visitor* visitor) const;

		/// 添加剪辑
		void addClip(const std::string& clipname, const AnimationClipData& clip);

		/// 取得可写的剪辑，不存在则返回 NULL
		AnimationClipData* getClip(const std::string& clipname);

		/// 取得只读的剪辑，不存在则返回 NULL
		const AnimationClipData* getClip(const std::string& clipname) const
		{
			return const_cast<AnimationClips*>(this)->getClip(clipname);
		}

	private:
		static float sGlobalDefaultAnimSpeed;

		struct impl;
		boost::shared_ptr<impl> mImpl;
	};
}}

