#pragma once
#include "../Components/StaticMesh.h"
#include "../Scene/Actor.h"
struct Collider;
class BoxShape;

class Platform : public Actor
{
public:

	Platform();
	Platform(const Vector3& position);
	~Platform() = default;

	void Update(float deltaTime) override;
	void Begin() override;
	void End() override;
};
