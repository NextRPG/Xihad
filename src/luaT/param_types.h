#pragma once

// 编译时常量字符串类型，当然仅作为输入。
#define literal(s) LString("" s, (sizeof(s)/sizeof(char))-1)

typedef struct lua_State lua_State;

// C函数，可以作为输入，也可以作为输出类型。
typedef int (* lua_CFunction) (lua_State*);

namespace luaT
{

	// 此类型仅能作为参数类型，没有方法可以以此从栈中弹出cclousure。
	typedef struct CClosure
	{
		lua_CFunction func;
		int n;
	} CClosure;

	// 此类型一般作为输入类型，即调用函数时只能为参数类型。
	// 否则（以此结构体取出栈中字符串），当字符串弹出栈中时，该字符串有着被gc回收的可能性。
	typedef struct LString
	{
		const char* value;
		size_t size;
		LString(const char* pCStr = NULL, size_t size = 0) : value(pCStr), size(size) { }
	} LString;

	// 调用函数时，需要以这两个枚举类型作为模板参数，让编译器生成正确代码。
	struct Nil
	{
		bool operator==(const Nil& o) const 
		{
			return true;
		}
	};

	struct Thread
	{

	};
}