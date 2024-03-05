#pragma once
#include "../Components/StaticMesh.h"
#include "../Scene/Actor.h"

class PickUpObject : public Actor
{
public:
	PickUpObject();
	~PickUpObject();

	void Update(float deltaTime) override;
	void Begin() override;
	void End() override;
	void BeginOverlap(size_t idOtherEntity);
	void EndOverlap(size_t idOtherEntity );
	/*StaticMesh mesh;*/
	size_t indexIgnored;
};
