#pragma once
#include "CppBase/ReferenceCounted.h"

namespace irrklang 
{
	enum E_STOP_EVENT_CAUSE;
}

namespace xihad { namespace audio
{
	typedef irrklang::E_STOP_EVENT_CAUSE E_STOP_EVENT_CAUSE;

	class AudioComponent;
	class AudioStopListener : public ReferenceCounted
	{
	public:
		virtual ~AudioStopListener() {}

		virtual void onSoundStopped(AudioComponent* sound, E_STOP_EVENT_CAUSE reason) = 0;
	};
}}

