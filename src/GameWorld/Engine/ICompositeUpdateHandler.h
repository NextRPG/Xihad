#pragma once
#include "UpdateHandler.h"

namespace xihad { namespace ngn
{
	/// 组合更新对象接口
	/**
	 * @see UpdateHandler
	 * @author etnlGD
	 * @date 2013年12月11日 15:19:46
	 */
	class ICompositeUpdateHandler : public virtual UpdateHandler
	{
	public:
		/// 测试是否包含指定更新对象
		/**
		 * @param handler 指定需要测试的更新对象
		 * @return true if contain
		 */
		virtual bool containsUpdateHandler(UpdateHandler* handler) const = 0;

		/// 附加更新对象
		/**
		 * handler 必须是一个非空并且没有被附加到其他 CompositeUpdateHandler 之上，才能够
		 * 成功附加。
		 * <p>如果本更新对象已经被 #start() ，那么当新的子对象被成功附加时，该子对象会立即被
		 * #start() 。
		 * <p>而如果将已经被 #start() 的对象附加到尚未 #start() 的组合更新对象，那么在组合
		 * 更新对象未 #start() 之前，该对象不会得到 #update() 的机会
		 * 
		 * @param handler 指定希望被附加的更新对象
		 * @return true if handler->getDestroyer() == this
		 */ 
		virtual bool appendUpdateHandler(UpdateHandler* handler) = 0;

		/// 移除更新子对象
		/**
		 * 此方法不会改变被移除子对象的状态
		 * 
		 * @return true if handler->getDestroyer() == nullptr
		 */
		virtual bool removeUpdateHandler(UpdateHandler* handler) = 0;

		/// 移除并摧毁子对象
		/**
		 * @see removeUpdateHandler()
		 * @return true if handler is was contained and is #destroy()-ed now
		 */
		bool deleteUpdateHandler(UpdateHandler* handler)
		{
			if (removeUpdateHandler(handler))
			{
				handler->destroy();
				return true;
			}

			return false;
		}
	};
}}

