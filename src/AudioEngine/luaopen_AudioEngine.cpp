#include <luaT/luaT.h>
#include "AudioComponent.h"

using namespace luaT;
namespace xihad { namespace audio 
{
	using namespace ngn;

	// TODO
	static int playMusic(lua_State* L)
	{

	}

	static int playSound(lua_State* L)
	{

	}

	static int addAudioStopListener(lua_State* L)
	{

	}

	static int removeAudioStopListener(lua_State* L)
	{

	}


	int luaopen_AudioComponents(lua_State* L)
	{
		luaT_defRegsBgn(audio)
			luaT_mnamedfunc(AudioComponent, setPaused),
			luaT_mnamedfunc(AudioComponent, isPaused),
			luaT_mnamedfunc(AudioComponent, stopAudio),
			luaT_mnamedfunc(AudioComponent, getVolume),
			luaT_mnamedfunc(AudioComponent, setVolume),
			luaT_mnamedfunc(AudioComponent, setPan),
			luaT_mnamedfunc(AudioComponent, getPan),
			luaT_mnamedfunc(AudioComponent, isLooped),
			luaT_mnamedfunc(AudioComponent, setLooped),
			luaT_mnamedfunc(AudioComponent, isFinished),
			luaT_mnamedfunc(AudioComponent, setMinDistance),
			luaT_mnamedfunc(AudioComponent, getMinDistance),
			luaT_mnamedfunc(AudioComponent, setMaxDistance),
			luaT_mnamedfunc(AudioComponent, getMaxDistance),
			luaT_mnamedfunc(AudioComponent, getPlayPosition),
			luaT_mnamedfunc(AudioComponent, setPlayPosition),
			luaT_mnamedfunc(AudioComponent, getPlayLength),
			luaT_mnamedfunc(AudioComponent, setPlaybackSpeed),
			luaT_mnamedfunc(AudioComponent, getPlaybackSpeed),

			// TODO 
		luaT_defRegsEnd
		MetatableFactory<AudioComponent, Component>::create(L, audio);

		return 0;
	}
}}


