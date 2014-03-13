#pragma once
#include "Engine/vector3d.h"
#include "Engine/matrix.h"

namespace xihad { namespace ngn
{
	/// 刚体变换类
	/**
	 * The order to various transform is fixeed as: 
	 *	Scale -> Rotate -> Translate
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

		void setFromMatrix(const Matrix&);

		/// 重置缩放
		void resetScale(const vector3df& scale = vector3df(1,1,1))
		{
			mScale = scale;
		}

		/// 取得当前缩放
		/**
		 * @see AxisIndex
		 * @return array of floating number (size = 3)
		 */
		const vector3df& getScale() const
		{
			return mScale;
		}
		
		/// 重置旋转，角度制
		void resetRotate(const vector3df& rotate = vector3df())
		{
			mRotation = rotate;
		}

		/// 取得当前旋转，角度制
		/**
		 * @see AxisIndex
		 * @return array of floating number (size = 3)
		 */
		const vector3df& getRotation() const
		{
			return mRotation;
		}
		
		/// 重置平移
		void resetTranslate(const vector3df& trans = vector3df())
		{
			mTranslate = trans;
		}

		/// 取得当前平移
		/**
		 * @see AxisIndex
		 * @return array of floating number (size = 3)
		 */
		const vector3df& getTranslation() const
		{
			return mTranslate;
		}

	private:
		vector3df mScale;
		vector3df mRotation;
		vector3df mTranslate;
	};
}}

