#pragma once
#include "dimension2d.h"
#include "vector3d.h"
#include "quaternion.h"

struct lua_State;
namespace xihad { namespace ngn
{
	/// 属性集合
	/**
	 * 通过字符串索引，从中取出存储的值。如果存储的值与期望的值类型不一致，或者指定键值不存在，那么
	 * 返回的是第二参数。需要注意的是，不能兼容类型也是不允许的，如不能从 int 值中获得 bool 值。
	 * 
	 * @author etnlGD
	 * @date 2013年12月13日 01:44:43
	 */
	class Properties
	{
	public:
		typedef bool			B;	///< 布尔
		typedef int				I;	///< 整型
		typedef float			F;	///< 浮点
		typedef const char*		S;	///< 字符串
		typedef dimension2df	D;	///< 二维大小
		typedef vector3df		V;	///< 三维向量
		typedef quaternion		Q;	///< 四元数

	public:
		/// 获取布尔值
		/**
		 * @param key 指定键值
		 * @param pDefault 如果指定键值不存在或类型不正确时，则返回此值
		 */
		virtual B getBool  (S key, B pDefault = B()) const = 0;

		/// 获取整型值
		/**
		 * @param key 指定键值
		 * @param pDefault 如果指定键值不存在或类型不正确时，则返回此值
		 */
		virtual I getInt   (S key, I pDefault = I()) const = 0;

		/// 获取浮点数
		/**
		 * @param key 指定键值
		 * @param pDefault 如果指定键值不存在或类型不正确时，则返回此值
		 */
		virtual F getFloat (S key, F pDefault = F()) const = 0;

		/// 获取二维浮点数
		/**
		 * @param key 指定键值
		 * @param pDefault 如果指定键值不存在或类型不正确时，则返回此值
		 */
		virtual D getDimen (S key, D pDefault = D()) const = 0;

		/// 获取三维浮点数
		/**
		 * @param key 指定键值
		 * @param pDefault 如果指定键值不存在或类型不正确时，则返回此值
		 */
		virtual V getVector(S key, V pDefault = V()) const = 0;

		/// 获取四元数
		/**
		 * @param key 指定键值
		 * @param pDefault 如果指定键值不存在或类型不正确时，则返回此值
		 */
		virtual Q getQuater(S key, Q pDefault = Q()) const = 0;

		/// 获取字符串
		/**
		 * @param key 指定键值
		 * @param pDefault 如果指定键值不存在或类型不正确时，则返回此值
		 */
		virtual S getString(S key, S pDefault = S()) const = 0;

		virtual void pushSelf(lua_State* L) const = 0;

		virtual ~Properties() {}
	};

}}

