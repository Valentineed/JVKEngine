#pragma once
#include "../Scene/Actor.h"

class Platform;

class PlatformSpawner : public Actor
{
public:
	PlatformSpawner();
	~PlatformSpawner() = default;
	void Update(float deltaTime) override;
	void BeginOverlap(size_t idOtherEntity);
	void SpawnPlatform();

private:
	bool m_bSpawned = false;
	Platform* m_spawnedP;
	bool m_bFinished = false;
};