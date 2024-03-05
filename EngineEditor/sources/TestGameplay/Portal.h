#pragma once

#include "../Scene/Actor.h"
class Portal :public Actor
{
public:
	Portal(const Vector3& pos);
	void BeginOverlap(size_t idOtherEntity);
	void EndOverlap(size_t idOtherEntity);

};