#pragma once

struct lua_State;
namespace xihad { namespace particle
{
	void luaopen_ParticleSystemNode(lua_State* L);
	void luaopen_ParticleSystemFactory(lua_State* L);
	void luaopen_ParticleEmitter(lua_State* L);
	void luaopen_ParticleIniter(lua_State* L);
	void luaopen_ParticleRenderer(lua_State* L);
	void luaopen_ParticleAffector(lua_State* L);

	inline void luaopen_AllParticleSystem(lua_State* L)
	{
		luaopen_ParticleSystemNode(L);
		luaopen_ParticleSystemFactory(L);
		luaopen_ParticleEmitter(L);
		luaopen_ParticleIniter(L);
		luaopen_ParticleRenderer(L);
		luaopen_ParticleAffector(L);
	}
}}

