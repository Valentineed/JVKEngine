#pragma once
#include <string_view>
#include "../StateMachine/States/Play/StatePlay.h"
struct Actor;

class Scene
{
public:
	Scene()
	{
		m_name = "no_name";
	}
	Scene(std::string_view name) :
		m_name(name)
	{
	}

	~Scene();

	void Begin();
	void BeginPlay();
	void Update(float deltaTime);
	void End();

	void InitStatePlay(StatePlay* stateplay);

	[[nodiscard]] Actor* GetActor(Actor* actor, unsigned int entityID);
	[[nodiscard]] Actor* GetActorName(Actor* root, std::string name);

	
	Actor* m_root = nullptr;
	std::string m_name;

	StatePlay* m_statePlay;
};
