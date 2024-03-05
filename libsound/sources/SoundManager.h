#pragma once
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string_view>



#include "fmod.h"
#include "fmod.hpp"
#include "fmod_common.h"
#include "fmod_codec.h"
#include "fmod_dsp.h"
#include "fmod_dsp_effects.h"
#include "fmod_output.h"
#include "SoundMode.h"
#include "SystemSound.h"
#include "libsound_Export.h"

class Sound3D;

namespace LibMath {
    struct Vector3;
}

struct Sound;

class SoundManager
{
public:

    libsound_API static SoundManager* GetInstance();
    SystemSound m_soundSystem;
    void libsound_API Init();

    void libsound_API Update();

    void libsound_API End();

    void libsound_API CreateSound2D(std::string_view path, SOUND_MODE mode, Sound& sound) const;

    void libsound_API ReleaseSound(Sound* sound);

    void libsound_API CreateSound(std::string_view path, Sound& sound) const;

    void libsound_API PlaySound2D(Sound* sound, bool paused = false) const;
	
    void libsound_API PlaySound3D(Sound3D* sound, bool paused = false) const;

    void libsound_API SetVolume(Sound* sound, float volume);
	
    void libsound_API Stop(Sound* sound);
	
    void libsound_API SetMute(Sound* sound, bool mute);

    bool libsound_API GetMute(Sound* sound);

    void libsound_API SetPause(Sound* sound, bool paused);

    bool libsound_API GetPause(Sound* sound);
	
    float libsound_API GetVolume(Sound* sound);

    float libsound_API GetFrequency(Sound* sound);
	
    void libsound_API SetFrequency(Sound* sound, float frequency);

    bool libsound_API GetLoop(Sound* sound);
	
    void libsound_API SetLoop(Sound* sound, bool loop);

    void libsound_API SetListener(const LibMath::Vector3& pos, const LibMath::Vector3& vel, const LibMath::Vector3& forward, const LibMath::Vector3& up);

    /**
     * @briefSets the global doppler scale, distance factor and log rolloff scale for all 3D sound in FMOD.
     *
     * @param Scaling factor for doppler shift.
     * @param Relative distance factor to FMOD units.
     * @param Scaling factor for 3D sound rolloff or attenuation
     */
    void libsound_API Set3DSettings(float dopplerScale, float distanceFactor, float attenuation) const;
private:
    /*Singleton*/
    SoundManager() = default;
    ~SoundManager() = default;
    SoundManager(SoundManager const&) = delete;
    SoundManager& operator=(SoundManager const&) = delete;

    FMOD_VECTOR m_listenerPos;
    FMOD_VECTOR m_listenerVel;
    FMOD_VECTOR m_listenerForward;
    FMOD_VECTOR m_listenerUp;
};
