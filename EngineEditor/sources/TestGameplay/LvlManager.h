#pragma once
#include "../Components/StaticMesh.h"
#include "../Scene/Actor.h"

struct Collider;
class SphereShape;

class LvlManager : public Actor
{
public:
	LvlManager();
	void Update(float deltaTime) override;
	void Begin() override;
	void End() override;

	void BeginOverlap(size_t idOtherEntity);
	std::vector<std::string> m_lvls;
	std::string m_currentLvl;
	std::vector<float> m_timesToReset;
private:
	bool m_changeScene = false;
	float m_time = 0.0f;

	float m_scale = 1.f;

	float m_timeToDestroy = 10.f;
};
