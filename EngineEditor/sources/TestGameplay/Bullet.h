#pragma once
#include "../Scene/Actor.h"

class Bullet : public Actor
{
public:
	Bullet(const Vector3& velocity, const Vector3& pos, Actor* actor);
	void Update(float deltaTime) override;
	void End() override;

	void BeginOverlap(size_t idOtherEntity);
	void Destroy();
	float m_time = 0.0f;
	float m_timeToDestroy = 10.f;
private:

	float m_scale = 3.5f;

	bool m_createPortal = false;
	
};
