#pragma once
#include "../Scene/Actor.h"


class Trampoline : public Actor
{
public:
	Trampoline();
	~Trampoline();
	
	void Update(float deltaTime) override;
	void Begin() override;
	void End() override;

	void OnBeginOverlap(size_t indexOtherCollider);
private:
	bool m_bApplyForce = false;
	Actor* m_box = nullptr;

};
