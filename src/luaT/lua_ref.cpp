#include "lua_ref.h"
#include "xassert.h"

 namespace luaT
 {

	 LuaRef LuaRef::fromTop( lua_State* L )
	 {
		 xassert(L != NULL);
		 int handle = lua_ref(L, true);
		 return LuaRef(L, handle);
	 }

	 LuaRef::LuaRef( const LuaRef& other )
	 {
		 gain(other.L, other.handle, other.refPtr);
	 }

	 LuaRef::LuaRef( lua_State* L, int handle)
	 {
		 int* refPtr;
		 if (handle != LUA_REFNIL && handle != LUA_NOREF)
			 refPtr = new int(0);
		 else
			 refPtr = NULL;

		 gain(L, handle, refPtr);
	 }

	 LuaRef::~LuaRef()
	 {
		 drop();
	 }

	 LuaRef& LuaRef::operator=( const LuaRef& other )
	 {
		 drop();
		 gain(other.L, other.handle, other.refPtr);
		 return *this;
	 }

	 void LuaRef::drop()
	 {
		 if (refPtr != NULL)
		 {
			 --(*refPtr);
			 if (*refPtr == 0)
			 {
				lua_unref(L, handle);
				delete refPtr;
			 }
		 }
	 }

	 void LuaRef::gain( lua_State* newState, int newHandle, int* newRefPtr )
	 {
		 this->L = newState;
		 this->handle = newHandle;
		 this->refPtr = newRefPtr;

		 if (refPtr) ++(*refPtr);
	 }

	 bool LuaRef::operator==( const LuaRef& other ) const
	 {
		this->pushSelf();
		other.pushOnto(L);
		int equal = lua_equal(L, -1, -2);
		lua_pop(L, 2);

		return equal==1;
	 }

 }

