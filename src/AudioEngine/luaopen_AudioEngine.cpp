#include <luaT/luaT.h>
#include "AudioComponent.h"
#include "CppBase/StringUtil.h"
#include "AudioStopListener.h"
#include "AudioComponentSystem.h"
#include "LuaGlobalVariable.h"

using namespace luaT;
namespace xihad { namespace audio 
{
	using namespace irrklang;
	using namespace ngn;
	
	static E_STREAM_MODE popStreamMode(lua_State* L, int idx)
	{
		if (lua_gettop(L) >= idx)
		{
			const char* modes[] = { "AutoDetect", "Streaming", "NoStreaming", 0 };
			luaT_variable(L, idx, const char*, mode);
			int midx = StringUtil::select(mode, modes);
			if (midx != -1)
				return (E_STREAM_MODE) midx;
			else 
				luaL_argerror(L, idx, "Error stream mode");
		}
		
		return ESM_AUTO_DETECT;
	}
	
	static int play2D(lua_State* L)
	{
		luaT_variable(L, 1, AudioComponent*, comp);
		luaT_variable(L, 2, const char*, fname);
		auto mode = popStreamMode(L, 3);
		
		comp->play2D(fname, mode);
		return 0;
	}

	static int play3D(lua_State* L)
	{
		luaT_variable(L, 1, AudioComponent*, comp);
		luaT_variable(L, 2, const char*, fname);
		auto mode = popStreamMode(L, 3);
		
		comp->play3D(fname, mode);
		return 0;
	}

// 	static int addAudioStopListener(lua_State* L)
// 	{
// 		class LuaAudioStopListener : public AudioStopListener
// 		{
// 		public:
// 			virtual void onSoundStopped(AudioComponent* sound, E_STOP_EVENT_CAUSE reason)
// 			{
// 				
// 			}
// 		};
// 	}
// 
// 	static int removeAudioStopListener(lua_State* L)
// 	{
// 
// 	}


	int luaopen_AudioComponents(lua_State* L, AudioComponentSystem* acs)
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
			luaT_lnamedfunc(play2D),
			luaT_lnamedfunc(play3D),
			
			// TODO 
			// AudioListener
		luaT_defRegsEnd
		MetatableFactory<AudioComponent, Component>::create(L, audio, 0);

		luaT_defRegsBgn(system)
			luaT_mnamedfunc(AudioComponentSystem, stopAllAudios),
			luaT_mnamedfunc(AudioComponentSystem, setAllAudiosPaused),
			luaT_mnamedfunc(AudioComponentSystem, setListenerPosition),
			luaT_mnamedfunc(AudioComponentSystem, setVolume),
			luaT_mnamedfunc(AudioComponentSystem, getVolume),
			luaT_mnamedfunc(AudioComponentSystem, setDefault3DAudioMinDistance),
			luaT_mnamedfunc(AudioComponentSystem, getDefault3DAudioMinDistance),
			luaT_mnamedfunc(AudioComponentSystem, setDefault3DAudioMaxDistance),
			luaT_mnamedfunc(AudioComponentSystem, getDefault3DAudioMaxDistance),
		luaT_defRegsEnd
		MetatableFactory<AudioComponentSystem>::create(L, system, 0);


		lua_getglobal(L, "_G");
		setField(L, -1, LUAT_G_AUDIO_SYSTEM, acs);

		return 0;
	}
}}


