#pragma once
#include "../Scene/Actor.h"
class Portal2 :public Actor
{
public:
	Portal2(const Vector3& pos);
	void BeginOverlap(size_t idOtherEntity);
	void EndOverlap(size_t idOtherEntity);
	
};
