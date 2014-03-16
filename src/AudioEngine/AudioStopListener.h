#pragma once
#include "CppBase/ReferenceCounted.h"

namespace irrklang 
{
	enum E_STOP_EVENT_CAUSE;
}

namespace xihad { namespace audio
{
	using namespace irrklang;

	class AudioComponent;
	class AudioStopListener : public ReferenceCounted
	{
	public:
		AudioStopListener() { XIHAD_MLD_NEW_OBJECT; }

		virtual ~AudioStopListener() { XIHAD_MLD_DEL_OBJECT; }

		/**
		 * @return if this listener want to continue listening.
		 */
		virtual bool onAudioStopped(AudioComponent* sound, E_STOP_EVENT_CAUSE reason) = 0;
	};
}}

