#pragma once
#include "../Scene/Actor.h"

class Button : public Actor
{
public:
	Button();

	void OnBeginOverlap(size_t idOtherActor);
	void OnEndOverlap(size_t idOtherActor);
	void BeginPlay() override;
	void DeleteTrap();
	Actor* m_trap = nullptr;
	bool m_bFinished = false;
};
