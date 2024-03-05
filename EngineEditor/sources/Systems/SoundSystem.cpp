#include "SoundSystem.h"

#include "SoundManager.h"
#include "../ECS/World.h"

void SoundSystem::Init()
{
	m_soundManager->Init();
}

void SoundSystem::Update()
{
	m_soundManager->Update();
}

void SoundSystem::End()
{
	for (auto sound : m_soundComponents)
	{
		sound.DeleteSound();
	}
}

SoundSystem::SoundSystem()
{
	m_soundManager = SoundManager::GetInstance();
}

void SoundSystem::Play(size_t index)
{
	m_soundManager->PlaySound3D(&m_soundComponents[index].Get3DSound());
}

float SoundSystem::GetVolume(size_t index)
{
	return m_soundManager->GetVolume(m_soundComponents[index].Get3DSound().GetSound());
}

bool SoundSystem::GetMute(size_t index)
{
	return m_soundManager->GetMute(m_soundComponents[index].Get3DSound().GetSound());
}

void SoundSystem::SetMute(size_t index, bool mute)
{
	m_soundManager->SetMute(m_soundComponents[index].Get3DSound().GetSound(), mute);
}

bool SoundSystem::GetFrequency(size_t index)
{
	return m_soundManager->GetFrequency(m_soundComponents[index].Get3DSound().GetSound());
}

void SoundSystem::SetFrequency(size_t index, float frequency)
{
	m_soundManager->SetFrequency(m_soundComponents[index].Get3DSound().GetSound(), frequency);
}

bool SoundSystem::GetLoop(size_t index)
{
	return m_soundManager->GetLoop(m_soundComponents[index].Get3DSound().GetSound());
}

void SoundSystem::SetLoop(size_t index, bool loop)
{
	m_soundManager->SetLoop(m_soundComponents[index].Get3DSound().GetSound(), loop);
}

void SoundSystem::Stop(const size_t index)
{
	m_soundManager->Stop(m_soundComponents[index].Get3DSound().GetSound());
}

bool SoundSystem::GetPause(size_t index)
{
	return m_soundManager->GetPause(m_soundComponents[index].Get3DSound().GetSound());
}

void SoundSystem::SetPause(size_t index, bool paused)
{
	m_soundManager->SetPause(m_soundComponents[index].Get3DSound().GetSound(), paused);
}

void SoundSystem::SetVolume(size_t index, float volume)
{
	m_soundManager->SetVolume(m_soundComponents[index].Get3DSound().GetSound(), volume);
}

void SoundSystem::RemoveSound(size_t index, int idEntity)
{
	if(m_soundComponents.size() - 1 == index)
	{
		m_soundComponents.pop_back();
	}
	else
	{
		m_soundComponents.back().indexSystem = index;
		std::iter_swap(m_soundComponents.begin() + index, m_soundComponents.end() - 1);
		m_soundComponents.pop_back();
	}
	
	if (idEntity != -1)
	{
		World::GetInstance().RemoveComponentID<SoundComponent>(idEntity);
	}
}

size_t SoundSystem::Register(std::string_view path, const LibMath::Vector3& pos, float maxDist, float minDist)
{
	const size_t index = m_soundComponents.size();
	m_soundComponents.emplace_back(path, pos, maxDist, minDist);
	m_soundComponents.back().indexSystem = index;
	return index;
}

SoundComponent* SoundSystem::GetSound(size_t index)
{
	return &m_soundComponents[index];
}
