#pragma once
#include <libgfx_Export.h>
#include <string>

class ActorSoundCompData
{
public:
	ActorSoundCompData() = default;

	//Getters
	libgfx_API std::string& GetPath();

	libgfx_API std::string& GetNewSoundPath();

	libgfx_API bool IsSoundChanged();
	
	libgfx_API bool& IsLooping();
	
	libgfx_API bool& IsMute();

	libgfx_API bool& PlayOnStart();
	
	libgfx_API float& GetVolume();
	
	libgfx_API float GetFrequency();

	//Setters
	void libgfx_API SetPath(std::string& path);

	void libgfx_API SetIsChangingSound(bool changing);

	void libgfx_API SetLooping(bool looping);

	void libgfx_API SetMute(bool muting);

	void libgfx_API SetVolume(float volume);

	void libgfx_API SetPlayOnStart(bool playonstart);

	void libgfx_API SetFrequency(float freq);

	void libgfx_API ChangeSound(std::string& path);

	

	

	

private:
	bool m_bIsLooping = false;
	bool m_bIsMute = false;
	bool m_bPlayOnStart = true;
	
	float m_volume = 0.1f;
	float m_frequency = 1.f;
	
	std::string m_path;	
	std::string m_newSoundPath;

	bool m_bIsSoundChanged = false;

	
};

