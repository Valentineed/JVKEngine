#pragma once
#include "../Components/StaticMesh.h"
#include "../Scene/Actor.h"
struct Collider;
class CapsuleShape;

class Capsule: public Actor
{
public:
	Capsule();
	~Capsule();
	void Update(float deltaTime) override;
	void Begin() override;
        void BeginPlay() override;
	void End() override;

	void BeginOverlap(size_t indexOtherCollider);
	void EndOverlap(size_t indexOtherCollider);
	void Fire();
	void Fire2();
	void PickObject();
	Actor* m_pickObject = nullptr;
	Actor* m_button = nullptr;
	bool m_bIsPick = false;
	bool m_bfirstTime = true;
	bool m_bOpenTrap;
	/*CapsuleShape* m_shape;
	Collider* m_collider;*/
};
