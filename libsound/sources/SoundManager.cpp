#include "SoundManager.h"

#include "Vector/Vector3.h"
#include "Sound.h"
#include <fmod_errors.h>

#include "Sound3D.h"

libsound_API SoundManager* SoundManager::GetInstance()
{
    static SoundManager instance;

    return &instance;
}

void SoundManager::Init()
{
    FMOD_RESULT result;
    result = FMOD::System_Create(&m_soundSystem.m_system); // Create the Studio System object.
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }

    result = m_soundSystem.m_system->init(512, FMOD_INIT_3D_RIGHTHANDED, 0);    // Initialize FMOD.
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void SoundManager::Update()
{
    m_soundSystem.m_system->update();
}

void SoundManager::End()
{
    m_soundSystem.m_system->release();
}

void SoundManager::CreateSound2D(std::string_view path, SOUND_MODE mode, Sound& sound) const
{
    m_soundSystem.m_system->createSound(path.data(), mode, nullptr, &sound.m_sound);
}

void SoundManager::ReleaseSound(Sound* sound)
{
    sound->m_sound->release();
}

void SoundManager::CreateSound(std::string_view path, Sound& sound) const
{
    m_soundSystem.m_system->createSound(path.data(), FMOD_CREATESTREAM, nullptr, &sound.m_sound);
}

void SoundManager::PlaySound2D(Sound* sound, bool paused) const
{
    if (!sound)
    {
        std::cout << "Create Sound early" << std::endl;
        return;
    }
    sound->m_chanel->setReverbProperties(0, 0);
    m_soundSystem.m_system->playSound(sound->m_sound, nullptr, paused, &sound->m_chanel);
}

void libsound_API SoundManager::PlaySound3D(Sound3D* sound, bool paused) const
{
    if (!sound)
    {
        std::cout << "Create Sound early" << std::endl;
        return;
    }
	
    m_soundSystem.m_system->playSound(sound->GetSound()->m_sound, nullptr, paused, &sound->GetSound()->m_chanel);
}

void SoundManager::SetVolume(Sound* sound, float volume)
{
    sound->m_chanel->setVolume(volume);
}

void SoundManager::Stop(Sound* sound)
{
    sound->m_chanel->stop();
}

void SoundManager::SetMute(Sound* sound, bool mute)
{
    sound->m_chanel->setMute(mute);
}

bool SoundManager::GetMute(Sound* sound)
{
    bool mute;
    sound->m_chanel->getMute(&mute);
    return mute;
}

void SoundManager::SetPause(Sound* sound, bool paused)
{
    sound->m_chanel->setPaused(paused);
}

bool libsound_API SoundManager::GetPause(Sound* sound)
{
    bool pause;
    sound->m_chanel->getPaused(&pause);
    return pause;
}

float libsound_API SoundManager::GetVolume(Sound* sound)
{
    float volume;
    sound->m_chanel->getVolume(&volume);
    return volume;
}

float libsound_API SoundManager::GetFrequency(Sound* sound)
{
    float frequency;
    sound->m_chanel->getFrequency(&frequency);
    return frequency;
}

void SoundManager::SetFrequency(Sound* sound, float frequency)
{
    sound->m_chanel->setFrequency(frequency);
}

bool libsound_API SoundManager::GetLoop(Sound* sound)
{
    int count;
    sound->m_sound->getLoopCount(&count);
	if(count == -1)
	{
        return true;
	}
    return false;
}

void SoundManager::SetLoop(Sound* sound, bool loop)
{
    if (loop)
    {
        sound->m_sound->setLoopCount(-1);
    }
    else
    {
        sound->m_sound->setLoopCount(0);

    }
}

void libsound_API SoundManager::SetListener(const LibMath::Vector3& pos, const LibMath::Vector3& vel, const LibMath::Vector3& forward, const LibMath::Vector3& up)
{
    m_listenerPos = { pos.x, pos.y, pos.z };
    m_listenerVel = { vel.x, vel.y, vel.z };
    m_listenerForward = { forward.x, forward.y, forward.z };
    m_listenerUp = { up.x, up.y, up.z };
    m_soundSystem.m_system->set3DListenerAttributes(0, &m_listenerPos, &m_listenerVel, &m_listenerForward, &m_listenerUp);
}

void libsound_API SoundManager::Set3DSettings(float dopplerScale, float distanceFactor, float attenuation) const
{
    m_soundSystem.m_system->set3DSettings(dopplerScale, distanceFactor, attenuation);
}
