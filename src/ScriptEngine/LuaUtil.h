#pragma once
#include <string>
namespace luaT
{
	class LuaRef;
}
struct lua_State;
namespace xihad { namespace script
{
	/// Lua 工具库
	class LuaUtil
	{
	public:
		enum { REQUIRE_OK = 0 };

		/// 调用 Lua 标准库的 require 函数
		static int require(lua_State* L, const char* libName);

		/// 统一格式输出错误
		static void outputErrorMessage(lua_State* L, const char* desc = nullptr, int msgIdx = -1);

		/// 返回指定位置元素是否可能是可以被调用的
		/**
		 * @return false 那么一定不可以被调用
		 *		   true  可能可以被调用（具体视元表有无 __call 方法）
		 */
		static bool iscallable(lua_State* L, int idx);

		/// 获取当前函数调用堆栈，调试时使用
		static std::string stacktrace(lua_State* L);

		/// 调用 Lua 对象的成员函数
		/**
		 * 错误处理为输出错误消息
		 * @see callSelf
		 */
		static void callSelfErr(luaT::LuaRef& table, const char* funcName, bool optional = true);

		/// 调用 Lua 对象的成员函数
		/**
		 * <pre>
		 *	table:funcName()
		 * </pre>
		 * 如果调用失败（返回 false ），那么错误消息会位于栈顶，否则栈不变
		 * 
		 * @param table 类型必须是表
		 * @param funcName 成员函数名称
		 * @param optional 如果该键对应的值不可被调用，那么直接返回
		 * @return true 如果调用成功或者没有调用
		 */
		static bool callSelf(luaT::LuaRef& table, const char* funcName, bool optional = true);

		/// 模拟调用类的静态成员函数，无多态
		/**
		 * <pre>
		 *	local func = rawget(table, funcName)
		 *	func()
		 * </pre>
		 * 如果调用失败（返回 false ），那么错误消息会位于栈顶，否则栈不变
		 * 
		 * @param L
		 * @param tidx		表所在位置
		 * @param funcName  静态函数名称
		 * @param optional  如果该键对应的值不可被调用，那么直接返回
		 * @return true 如果调用成功或者没有调用 
		 */
		static bool rawCall(lua_State* L, int tidx, const char* funcName, bool optional = true);
	};
}}
