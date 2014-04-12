#pragma once

#ifdef CEGUI_EXPORTS
#	define CEGUI_HANDLER_API __declspec(dllexport)
#else
#	define CEGUI_HANDLER_API __declspec(dllimport)
#endif
