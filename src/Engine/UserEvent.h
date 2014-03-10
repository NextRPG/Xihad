#pragma once
#include <irrlicht/IEventReceiver.h>

namespace irr
{
	enum EKEY_CODE;
}

namespace xihad { namespace ngn
{
	typedef irr::SEvent::SMouseInput MouseEvent;

	typedef irr::SEvent::SKeyInput KeyEvent;

	typedef irr::EKEY_CODE KeyCode;

	struct KeyCode_helper
	{
		/// 将 KeyCode 转换为字符串形式
		/**
		 * 所有的字符串均由大写字母、数字、下划线组成。如果不能转换，返回 UNKNOWN
		 */
		static const char* toString(KeyCode code);
	};
}}
