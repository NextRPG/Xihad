#pragma once
#include "UserEvent.h"
#include "CppBase/ReferenceCounted.h"

namespace xihad { namespace ngn
{
	//! 用户事件处理
	class UserEventReceiver : public ReferenceCounted
	{
	public:
		UserEventReceiver() { XIHAD_MLD_NEW_OBJECT; }

		virtual ~UserEventReceiver() { XIHAD_MLD_DEL_OBJECT; }

		//! 处理键盘事件
		/**
		 * 一般会存在多个事件接收对象，栈顶的对象没有处理事件则该事件会一直传播到栈底
		 * 
		 * @param event 键盘事件信息
		 * @param argFromPreviousReceiver 如果当前对象是第一个接收事件者，则为0；否则为上一个
		 *			事件接收者的返回值
		 * @return 如果处理了事件，则返回 0；否则，返回的任意非零值会被当作参数传递给下一个事件接收者
		 */
		virtual int onKeyEvent(const KeyEvent& event, int argFromPreviousReceiver) = 0;

		//! 处理鼠标事件
		/**
		 * 一般会存在多个事件接收对象，栈顶的对象没有处理事件则该事件会一直传播到栈底
		 * 
		 * @param event 鼠标事件信息
		 * @param argFromPreviousReceiver 如果当前对象是第一个接收事件者，则为0；否则为上一个
		 *			事件接收者的返回值
		 * @return 如果处理了事件，则返回 0；否则，返回的任意非零值会被当作参数传递给下一个事件接收者
		 */
		virtual int onMouseEvent(const MouseEvent& event, int argFromPreviousReceiver) = 0;
	};
}}

