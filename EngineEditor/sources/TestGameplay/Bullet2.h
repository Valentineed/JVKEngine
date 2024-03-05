#pragma once
#include "../Scene/Actor.h"
class Bullet2 : public Actor
{
public:
	Bullet2(const Vector3& velocity, const Vector3& pos, Actor* actor);

	void Update(float deltaTime) override;
	void End() override;
	void BeginOverlap(size_t idOtherEntity);
	void Destroy();
	bool m_bDestroy = false;
private:
	size_t m_count = 0;
	float m_time = 0.0f;

	float m_scale = 3.5f;

	float m_timeToDestroy = 10.f;
	bool m_createPortal = false;
	bool m_changeVelocity = false;
};
