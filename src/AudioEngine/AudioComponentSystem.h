#pragma once
#include "Engine\BaseComponentSystem.h"
#include <boost\scoped_ptr.hpp>

namespace xihad { namespace audio 
{
	class AudioComponentSystem : public ngn::BaseComponentSystem
	{
	public:
		explicit AudioComponentSystem(const ngn::InheritanceTree& tree);

		virtual ngn::Component* create(const std::string& typeName, ngn::GameObject& hostObject, 
			const ngn::Properties& param = ngn::NullProperties()) override;

		/// Stops all currently playing sounds.
		virtual void stopAllAudios();

		/// Pauses or unpauses all currently playing sounds.
		virtual void setAllAudiosPaused(bool pause);

		/// Sets the current listener 3d position.
		/** 
		 * When playing sounds in 3D, updating the position of the listener every frame should be
		 * done using this function.
		 *
		 * @param pos Position of the camera or listener.
		 * @param lookdir Direction vector where the camera or listener is looking into. If you have a 
		 * 				camera position and a target 3d point where it is looking at, this would be cam->getTarget() - cam->getAbsolutePosition().
		 * @param upvector Vector pointing 'up', so the engine can decide where is left and right. 
		 *				This vector is usually (0,1,0).
		 */
		virtual void setListenerPosition(
			const ngn::vector3df& position = ngn::vector3df(0, 0, 0), 
			const ngn::vector3df& lookdir = ngn::vector3df(0, 1, 0), 
			const ngn::vector3df& upVec = ngn::vector3df(0, 1, 0));

		/// Sets master sound volume. This value is multiplied with all sounds played.
		/** 
		 * @param volume 0 (silent) to 1.0f (full volume) 
		 */
		virtual void setVolume(float volume);

		/// Returns master sound volume.
		/**
		 * A value between 0.0 and 1.0. Default is 1.0. Can be changed using setSoundVolume(). 
		 */
		virtual float getVolume();

		/// Sets the default minimal distance for 3D sounds.
		/** 
		 * This value influences how loud a sound is heard based on its distance.
		 * See ISound::setMinDistance() for details about what the min distance is.
		 * It is also possible to influence this default value for every sound file 
		 * using ISoundSource::setDefaultMinDistance().
		 * This method only influences the initial distance value of sounds. For changing the
		 * distance after the sound has been started to play, use ISound::setMinDistance() and ISound::setMaxDistance().
		 * 
		 * @param minDistance Default minimal distance for 3d sounds. The default value is 1.0f.
		 */
		virtual void setDefault3DAudioMinDistance(float minDistance);

		/// Returns the default minimal distance for 3D sounds.
		/** 
		 * This value influences how loud a sound is heard based on its distance.
		 * You can change it using setDefault3DSoundMinDistance().
		 * See ISound::setMinDistance() for details about what the min distance is.
		 * It is also possible to influence this default value for every sound file 
		 * using ISoundSource::setDefaultMinDistance().
		 * 
		 * @return Default minimal distance for 3d sounds. The default value is 1.0f. 
		 */
		virtual float getDefault3DAudioMinDistance();

		/// Sets the default maximal distance for 3D sounds.
		/** 
		 * Changing this value is usually not necessary. Use setDefault3DSoundMinDistance() instead.
		 * Don't change this value if you don't know what you are doing: This value causes the sound
		 * to stop attenuating after it reaches the max distance. Most people think that this sets the
		 * volume of the sound to 0 after this distance, but this is not true. Only change the
		 * minimal distance (using for example setDefault3DSoundMinDistance()) to influence this.
		 * See ISound::setMaxDistance() for details about what the max distance is.
		 * It is also possible to influence this default value for every sound file 
		 * using ISoundSource::setDefaultMaxDistance().
		 * This method only influences the initial distance value of sounds. For changing the
		 * distance after the sound has been started to play, use ISound::setMinDistance() and ISound::setMaxDistance().
		 *
		 * @param maxDistance Default maximal distance for 3d sounds. The default value is 1000000000.0f. 
		 */
		virtual void setDefault3DAudioMaxDistance(float maxDistance);

		/// Returns the default maximal distance for 3D sounds.
		/** 
		 * This value influences how loud a sound is heard based on its distance.
		 * You can change it using setDefault3DSoundmaxDistance(), but 
		 * changing this value is usually not necessary. This value causes the sound
		 * to stop attenuating after it reaches the max distance. Most people think that this sets the
		 * volume of the sound to 0 after this distance, but this is not true. Only change the
		 * minimal distance (using for example setDefault3DSoundMinDistance()) to influence this.
		 * See ISound::setMaxDistance() for details about what the max distance is.
		 * It is also possible to influence this default value for every sound file 
		 * using ISoundSource::setDefaultMaxDistance().
		 *
		 * @return Default maximal distance for 3d sounds. The default value is 1000000000.0f. 
		 */
		virtual float getDefault3DAudioMaxDistance();

	protected:
		virtual ~AudioComponentSystem();

		virtual void onStart();

		virtual void onUpdate( const ngn::Timeline& );

		virtual void onStop();

	private:
		struct impl;
		boost::scoped_ptr<impl> mImpl;
	};

}}