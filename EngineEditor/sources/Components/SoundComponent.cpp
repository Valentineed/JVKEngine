#include "SoundComponent.h"

#include "../ECS/World.h"
#include "../Systems/SoundSystem.h"

SoundComponent::SoundComponent(std::string_view path, const LibMath::Vector3& pos, float maxDist, float minDist): m_pos(pos),
m_maxDist(maxDist), m_minDist(minDist), m_path(path)
{
	m_sound = new Sound3D(path, pos, maxDist, minDist);

	m_system = World::GetInstance().GetSystem<SoundSystem>();
}

float SoundComponent::GetMaxDistance() const
{
	return m_maxDist;
}

float SoundComponent::GetMinDistance() const
{
	return m_minDist;
}

void SoundComponent::DeleteSound()
{
	delete m_sound;
}

void SoundComponent::Play()
{
	m_system->Play(indexSystem);
}

Vector3& SoundComponent::GetPosition()
{
	return m_pos;
}

float SoundComponent::GetVolume() const
{
	return m_system->GetVolume(indexSystem);
}

void SoundComponent::ActiveInverseMod() const
{
	m_sound->ActiveInverseMod();
}

void SoundComponent::SetMaxDistance(float maxDist)
{
	m_maxDist = maxDist;
	m_sound->SetAllSoundAttributes(m_pos, m_maxDist, m_minDist);
}

void SoundComponent::SetMinDistance(float minDist)
{
	m_minDist = minDist;
	m_sound->SetAllSoundAttributes(m_pos, m_maxDist, m_minDist);
}

void SoundComponent::SetPosition(const Vector3& pos,float deltaTime)
{
	Vector3 velocity;
	if(deltaTime != 0.f)
	{
		velocity = (pos - m_pos) / deltaTime;
	}
	else
	{
		velocity = Vector3::Zero;
	}
	m_pos = pos;
	m_sound->SetPos(pos, velocity);
}

float SoundComponent::GetFrequency() const
{
	return m_system->GetFrequency(indexSystem);
}

void SoundComponent::SetFrequency(float frequency) const
{
	m_system->SetFrequency(indexSystem, frequency);
}

bool SoundComponent::GetLoop()const
{
	return m_system->GetLoop(indexSystem);
}

void SoundComponent::SetLoop(bool loop)
{
	m_system->SetLoop(indexSystem, loop);
	
	m_bIsLooping = loop;
}

bool SoundComponent::GetPause()const
{
	return m_system->GetPause(indexSystem);
}

void SoundComponent::SetPause(bool paused) const
{
	m_system->SetPause(indexSystem, paused);
}

void SoundComponent::SetVolume(float volume)
{
	m_system->SetVolume(indexSystem, volume);

	m_volume = volume;
}

void SoundComponent::Active3DLinear() const
{
	m_sound->Active3DLinear();
}

void SoundComponent::Active3DSquareLinear() const
{
	m_sound->Active3DSquareLinear();
}

bool SoundComponent::GetMute()const
{
	return m_system->GetMute(indexSystem);
}

void SoundComponent::SetMute(bool mute)
{
	m_bIsMute = mute;
	
	return m_system->SetMute(indexSystem, mute);

	
}

void SoundComponent::Stop() const
{
	m_system->Stop(indexSystem);
}

Sound3D& SoundComponent::Get3DSound()
{
	return *m_sound;
}

std::string& SoundComponent::GetPath()
{
	return m_path;
}

void SoundComponent::SetPath(std::string& path)
{
	m_path = path;
}

void SoundComponent::ChangeSound(std::string& path)
{
	delete m_sound;
	m_sound = new Sound3D(path, m_pos, m_maxDist, m_minDist);
	m_path = path;
}
