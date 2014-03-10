#pragma once

namespace xihad { namespace ngn
{
	/// 进程操作函数集合
	class Process
	{
	public:
		/// 进程休眠
		/**
		 * @param seconds 休眠时间，秒为单位
		 */
		static void sleep(float seconds);
	};
}}

