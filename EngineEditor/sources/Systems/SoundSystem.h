#pragma once
#include <vector>

#include "../Components/SoundComponent.h"
#include "../ECS/System.h"
/**
 *@brief Manage all 3D sounds
 * 
 */
class SoundSystem : public System
{
public:
	/**
	*@brief Init Fmod
	* 
	*/
	void Init();

	void Update();

	void End();

	SoundSystem();
	/**
	*@brief Play a sound
	*
	*@param index in system
	*/
	void Play(size_t index);
	/**
	*@brief Get Mute Sound
	*
	*@param index in system
	*@return a boolean mute
	*/
	bool GetMute(size_t index);
	/**
	*@brief Set Mute Sound
	* @param index in system
	*@param this boolean set mute
	*/
	void SetMute(size_t index, bool mute);
	/**
	*@brief Get Frequency Sound
	*
	*@param index in system
	*@return a Frequency in HZ
	*/
	bool GetFrequency(size_t index);
	/**
	*@brief Sets the channel's frequency or playback rate, in HZ. 
	*
	* @param index in system
	* @param A frequency value in HZ.
	*/
	void SetFrequency(size_t index, float frequency);
	/**
	*@brief Get Loop state
	*
	*@param index in system
	*@return if loop or no
	*/
	bool GetLoop(size_t index);
	/**
	*@brief Set a pause in sound
	*
	* @param index in system
	* @param lopp or no
	*/
	void SetLoop(size_t index, bool loop);
	/**
	*@brief  Stop a Sound
	* @param index in system
	*/
	void Stop(const size_t index);
	/**
	*@brief  Get pause state
	* @param index in system
	*@return return a state of pause
	*/
	bool GetPause(size_t index);
	/**
	*@brief Set a pause in sound
	*
	* @param index in system
	* @param pause or no
	*/
	void SetPause(size_t index, bool paused);
	/**
	 *@brief  Get a volume off Sound
	* @param index in system
	*@return return a volume of Sound
	*/
	float GetVolume(size_t index);
	/**
	*@brief Set a volume off sound
	*
	* @param index in system
	* @param volume to set
	*/
	void SetVolume(size_t index, float volume);
	/**
	* @brief Remove a component in System
	* @param index in system
	*/
	void RemoveSound(size_t index, int idEntity);
	/**
	* @brief Register a component in System
	* @param component sound 
	*/
	[[nodiscard]] size_t Register(std::string_view path, const LibMath::Vector3& pos, float maxDist, float minDist);
	/**
	* @brief Get a sound component
	* @param index in system
	*/
	[[nodiscard]] SoundComponent* GetSound(size_t index);
	
private:
	SoundManager* m_soundManager = nullptr;
	/*
	 * @brief Store all 3D sounds
	 */
	std::vector<SoundComponent>m_soundComponents;

	
};
