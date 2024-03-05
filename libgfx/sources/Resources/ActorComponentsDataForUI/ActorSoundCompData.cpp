#include "ActorSoundCompData.h"

#include <windows.h>

libgfx_API std::string& ActorSoundCompData::GetPath()
{
	return  m_path;
}

libgfx_API std::string& ActorSoundCompData::GetNewSoundPath()
{
	return m_newSoundPath;
}

void libgfx_API ActorSoundCompData::SetPath(std::string& path)
{
	m_path = path;
}

void libgfx_API ActorSoundCompData::ChangeSound(std::string& path)
{
	m_newSoundPath = path;

	if (m_newSoundPath.find(".wav") != std::string::npos || m_newSoundPath.find(".mp3") != std::string::npos)
	{
		m_bIsSoundChanged = true;
	}
	else
	{
		m_newSoundPath.clear();
		MessageBox(NULL, "The path given was not a wav or mp3 file", "Error changing new sound", MB_OK);
	}
}

libgfx_API bool ActorSoundCompData::IsSoundChanged()
{
	return m_bIsSoundChanged;
}

libgfx_API bool& ActorSoundCompData::IsLooping()
{
	return m_bIsLooping;
}

libgfx_API bool& ActorSoundCompData::IsMute()
{
	return m_bIsMute;
}

libgfx_API bool& ActorSoundCompData::PlayOnStart()
{
	return m_bPlayOnStart;
}

libgfx_API float& ActorSoundCompData::GetVolume()
{
	return m_volume;
}

libgfx_API float ActorSoundCompData::GetFrequency()
{
	return m_frequency;
}

void libgfx_API ActorSoundCompData::SetIsChangingSound(bool changing)
{
	m_bIsSoundChanged = changing;
}

void libgfx_API ActorSoundCompData::SetLooping(bool looping)
{
	m_bIsLooping = looping;
}

void libgfx_API ActorSoundCompData::SetMute(bool muting)
{
	m_bIsMute = muting;
}

void libgfx_API ActorSoundCompData::SetVolume(float volume)
{
	m_volume = volume;
}

void libgfx_API ActorSoundCompData::SetPlayOnStart(bool playonstart)
{
	m_bPlayOnStart = playonstart;
}

void libgfx_API ActorSoundCompData::SetFrequency(float freq)
{
	m_frequency = freq;
}
