#pragma once

namespace xihad { namespace ngn
{
	/// 刚体变换类
	/**
	 * The order to various transform is fixeed as: 
	 *	Scale -> Rotate -> Translate
	 * <p>Therefore, result of translate then rotate is the same as rotate then
	 * translate
	 * 
	 * @author etnlGD
	 * @date 2013年12月10日 11:00:22
	 */
	class Transform
	{
	public:
		/// 坐标轴索引
		/**
		 * 用于索引返回的数组
		 * @see getScale(), getRotation(), getTranslation()
		 */
		enum AxisIndex
		{
			X_AXIS_INDEX = 0,	///< 获取X轴对应值的索引
			Y_AXIS_INDEX = 1,	///< 获取Y轴对应值的索引
			Z_AXIS_INDEX = 2,	///< 获取Z轴对应值的索引
		};

	public:
		/**
		 * 默认状态：scale=vec3(1), tranlation=vec3(0), rotation=vec3(0)
		 */
		Transform();

#if 0
		/// 在当前变换上级联缩放
		void concatScale(float px, float py, float pz)
		{
			resetScale(
				mScale[X_AXIS] * px,
				mScale[Y_AXIS] * py,
				mScale[Z_AXIS] * pz);
		}

		/// 在当前变换上级联缩放
		void concatUniScale(float s)
		{
			this->concatScale(s, s, s);
		}

		/// 在当前变换上级联旋转，角度制
		void concatRotate(float px, float py, float pz)
		{
			resetRotate(
				mRotation[X_AXIS] + px, 
				mRotation[Y_AXIS] + py, 
				mRotation[Z_AXIS] + pz);
		}

		/// 在当前变换上级联平移
		void concatTranslate(float px, float py, float pz)
		{
			resetTranslate(
				mTranslate[X_AXIS] + px, 
				mTranslate[Y_AXIS] + py, 
				mTranslate[Z_AXIS] + pz);
		}
#endif

		/// 重置缩放
		void resetScale(float sx = 1, float sy = 1, float sz = 1);

		/// 取得当前缩放
		/**
		 * @see AxisIndex
		 * @return array of floating number (size = 3)
		 */
		const float* getScale() const
		{
			return mScale;
		}
		
		/// 重置旋转，角度制
		void resetRotate(float rx = 0, float ry = 0, float rz = 0);

		/// 取得当前旋转，角度制
		/**
		 * @see AxisIndex
		 * @return array of floating number (size = 3)
		 */
		const float* getRotation() const
		{
			return mRotation;
		}
		
		/// 重置平移
		void resetTranslate(float tx = 0, float ty = 0, float tz = 0);

		/// 取得当前平移
		/**
		 * @see AxisIndex
		 * @return array of floating number (size = 3)
		 */
		const float* getTranslation() const
		{
			return mTranslate;
		}

	private:
		float mScale[3];
		float mRotation[3];
		float mTranslate[3];
	};
}}

