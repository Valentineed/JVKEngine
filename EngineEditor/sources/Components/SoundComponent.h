#pragma once
#include <string_view>

#include "Vector/Vector3.h"
#include "Sound3D.h"

class SoundSystem;
using namespace LibMath;
struct SoundComponent
{
	SoundComponent() = default;
	SoundComponent(std::string_view path, const LibMath::Vector3& pos, float maxDist, float minDist);
	~SoundComponent() = default;
	[[nodiscard]] float GetMaxDistance() const;
	[[nodiscard]] float GetMinDistance() const;

	void DeleteSound();
	
	/**
	*@brief  Play the Sound
	*/
	void Play();
	/**
	*@brief  Get a component position
	*@return return a position of Sound in Vector3
	*/
	[[nodiscard]] Vector3& GetPosition();
	/**
	*@brief  Get a component volume
	*@return return a volume of Sound
	*/
	[[nodiscard]] float GetVolume() const;
	/**
	*@brief Set a volume off sound
	*
	* @param volume to set
	*/
	void SetVolume(float volume);
	/**
	*@brief Default mod of 3D sound
	* 
	*/
	void ActiveInverseMod() const;
	/**
	*@brief It allows the 'mindistance' and 'maxdistance' settings to change the attenuation behavior
	*to fading linearly between the two distances
	* 
	*/
	void Active3DLinear() const;
	/**
	*@brief It allows the 'mindistance' and 'maxdistance' settings to change the attenuation behavior
	*to fading linearly between the two distances
	*
	*/
	void Active3DSquareLinear() const;
	/**
	*@brief set a distance from the centerpoint beyond which the reverb will have no effect.
	*
	* @param max distance of sound
	*/
	void SetMaxDistance(float maxDist);
	/**
	*@brief Set a distance from the centerpoint within which the reverb will have full effect..
	*
	* @param max distance of sound
	*/
	void SetMinDistance(float minDist);
	/**
	*@brief Set position of Sound
	*
	* @param position in world
	* @param deltaTime
	*/
	void SetPosition(const Vector3& pos, float deltaTime);
	/**
	*@brief Get Frequency Sound
	*
	*@return a Frequency in HZ
	*/
	[[nodiscard]] float GetFrequency() const;
	/**
	*@brief Sets the channel's frequency or playback rate, in HZ.
	*
	* @param A frequency value in HZ.
	*/
	void SetFrequency(float frequency) const;
	/**
	*@brief Get Loop state
	*
	*@return if loop or no
	*/
	[[nodiscard]] bool GetLoop()const;
	/**
	*@brief Set a pause in sound
	*
	* @param lopp or no
	*/
	void SetLoop(bool loop);
	/**
	*@brief  Get pause state
	*
	* @return return a state of pause
	*/
	[[nodiscard]] bool GetPause()const;
	/**
	*@brief Set a pause in sound
	*
	* @param pause or no
	*/
	void SetPause(bool paused) const;
	/**
	*@brief Get Mute Sound
	*
	*@return a boolean mute
	*/
	[[nodiscard]] bool GetMute() const;
	/**
	*@brief Set Mute Sound
	*
	*@param this boolean set mute
	*/
	void SetMute(bool mute);
	/**
	*@brief Stop a sound
	*/
	void Stop()const;
	
	/**
	 * @brief Get a 3DSound use in SoundSystem
	 */
	Sound3D& Get3DSound();

	/**
	*@brief Get Sound path
	*
	*@return a std::string
	*/
	[[nodiscard]] std::string& GetPath();

	/**
	*@brief Set Sound PAth
	*
	*@param The path string
	*/
	void SetPath(std::string& path);

	/**
	*@brief Change the current sound to be played
	*
	*@param The path string of the sound
	*/
	void ChangeSound(std::string& path);
	
	size_t indexSystem = -1;

	bool m_bIsLooping = true;
	bool m_bIsMute = false;

	float m_volume = 0.1f;
	float m_frequency = 1.f;

	bool m_bPlayOnWake = true;
private:
	Vector3 m_pos;
	float m_maxDist = 100;
	float m_minDist = 10;

	Sound3D* m_sound;
	SoundSystem* m_system = nullptr;

	std::string m_path;


	
};