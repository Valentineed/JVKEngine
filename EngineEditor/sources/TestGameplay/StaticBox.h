#pragma once
#include "../Components/StaticMesh.h"
#include "../Scene/Actor.h"
struct Collider;
class BoxShape;

class StaticBox : public Actor
{
public:

	StaticBox();
	~StaticBox();

	void Update(float deltaTime) override;
	void Begin() override;
	void End() override;
	void BeginOverlap(size_t idOtherEntity);
	void EndOverlap(size_t idOtherEntity);
	bool m_bChange = false;
	bool m_bInCollider = false;
	bool m_bFirstTime = true;
	Actor* m_pickUpObject=nullptr;
	
};
