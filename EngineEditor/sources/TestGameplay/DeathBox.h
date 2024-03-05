#pragma once
#include "../Scene/Actor.h"

class DeathBox : public Actor
{
public:
	DeathBox(); 

	void BeginOverlap(size_t idOtherEntity);
	std::vector<Vector3> m_positions;
	std::vector<Vector3> m_scales;
};
