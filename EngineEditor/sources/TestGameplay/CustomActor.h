#pragma once
#include "../Scene/Actor.h"

class CustomActor : public Actor
{
public:
	CustomActor(std::string_view objPath, std::string_view texturePath);
	~CustomActor() = default;

	void Update(float deltaTime) override;
	void Begin() override;
	void End() override;

private:
	bool m_bCollider = false;
};