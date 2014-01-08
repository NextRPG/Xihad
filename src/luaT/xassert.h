#pragma once

#ifdef _DEBUG
#define xassert(expression) { if (!(expression)) *(int*)(0) = 1; }
#else
#define xassert(expression) 
#endif

#define precondition(expression) xassert(expression)
#define postcondition(expression) xassert(expression)

