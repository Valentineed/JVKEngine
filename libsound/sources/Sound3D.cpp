#include "Sound3D.h"

#include <fmod_common.h>

#include "SoundManager.h"

#include "Vector/Vector3.h"
#include "fmod.hpp"
#include "SystemSound.h"
#include "Sound.h"

Sound3D::~Sound3D()
{
	m_soundWrap.m_sound->release();
}

Sound3D::Sound3D(std::string_view path)
{
	m_manager = SoundManager::GetInstance();
	FMOD_RESULT result;
	result = m_manager->m_soundSystem.m_system->createStream(path.data(), FMOD_3D, 0, &m_soundWrap.m_sound);
	if (result != FMOD_OK)
	{
		std::cerr << "Fail create A Sound 3D" << std::endl;
	}
}

Sound3D::Sound3D(std::string_view path, const LibMath::Vector3& pos, float maxDist, float minDist)
{
	m_manager = SoundManager::GetInstance();
	FMOD_RESULT result;
	
	result = m_manager->m_soundSystem.m_system->createStream(path.data(), FMOD_3D, 0, &m_soundWrap.m_sound);
	if (result != FMOD_OK)
	{
		std::cerr << "Fail create A Sound 3D" << std::endl;
	}
	m_soundWrap.m_sound->set3DMinMaxDistance(minDist, maxDist);
	m_soundPos = { pos.x, pos.y, pos.z };
	m_soundWrap.m_sound->setMode(FMOD_LOOP_NORMAL);
	//m_soundWrap.m_chanel->set3DAttributes(&m_soundPos, &m_velocity);
}

void Sound3D::CreateReverb(const LibMath::Vector3& pos, float maxDist, float minDist)
{
	m_maxDist = maxDist;
	m_minDist = minDist;
	FMOD_RESULT result;
	result = m_manager->m_soundSystem.m_system->createReverb3D(&m_reverb);
	if (result != FMOD_OK)
	{
		std::cerr << "Fail create Reverb" << std::endl;
	}

	FMOD_REVERB_PROPERTIES prop2 = FMOD_PRESET_CONCERTHALL;
	m_reverb->setProperties(&prop2);

	m_soundPos = { pos.x,pos.y, pos.z };
	m_reverb->set3DAttributes(&m_soundPos, m_minDist, m_maxDist);
}

void Sound3D::SetPos(const LibMath::Vector3& pos, const LibMath::Vector3& vel)
{
	m_soundPos = { pos.x,pos.y, pos.z };
	m_velocity = { vel.x, vel.y, vel.z };
	//m_reverb->set3DAttributes(&m_soundPos, m_minDist, m_maxDist);
	m_soundWrap.m_chanel->set3DAttributes(&m_soundPos, &m_velocity);
}

void Sound3D::SetAllSoundAttributes(const LibMath::Vector3& pos, float maxDist, float minDist)
{
	m_maxDist = maxDist;
	m_minDist = minDist;
	m_soundWrap.m_chanel->get3DAttributes(&m_soundPos, &m_velocity);
	m_soundPos = { pos.x,pos.y, pos.z };
	m_reverb->set3DAttributes(&m_soundPos, m_minDist, m_maxDist);
	m_soundWrap.m_sound->set3DMinMaxDistance(m_minDist, m_maxDist);
	m_soundWrap.m_chanel->set3DAttributes(&m_soundPos, &m_velocity);
}

void Sound3D::Active3DLinear() const
{
	m_soundWrap.m_sound->setMode(FMOD_3D_LINEARROLLOFF);
}

void Sound3D::ActiveInverseMod()const
{
	m_soundWrap.m_sound->setMode(FMOD_3D_INVERSEROLLOFF);
}

void Sound3D::Active3DSquareLinear() const
{
	m_soundWrap.m_sound->setMode(FMOD_3D_LINEARSQUAREROLLOFF);
}

Sound* Sound3D::GetSound()
{
	return &m_soundWrap;
}
