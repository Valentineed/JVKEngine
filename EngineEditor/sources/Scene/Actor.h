#pragma once
#include <cstdint>
#include <vector>



#include "Defines.h"
#include "../Components/Camera.h"
#include "../Components/ScriptComponent.h"
#include "../Components/StaticMesh.h"
#include "../ECS/World.h"
#include "../ECS/CommonTypes.h"
#include "../Systems/LightSystem.h"

class StaticMeshSystem;
class SoundSystem;
class ScriptingSystem;
class PhysixSystem;
class CameraSystem;
struct Transform;
class TransformSystem;
struct Collider;
struct LightComponent;
struct SoundComponent;
class World;

struct Actor
{
public:
	Actor(const std::string& name = "Actor");
	virtual ~Actor() = default;
	
	bool DeleteThis(bool scene = false);
	
	virtual void BeginPlay();
	virtual void Begin();
	virtual void Update(float deltaTime);
	virtual void End();

	void AddChild(Actor* actor);

	/*---Component util Function---*/	
	template<typename T>
	void AddComponent(size_t indexSystem);
	
	template<typename T>
	size_t GetComponent();
	/*----------------------*/
	
	void RemoveLastChild();
	void PopLastChild();
	bool Remove();
	void RemoveChild(size_t index);
	
	void SetTransform(Transform& trans);


	Actor* GetParent();
	void SetParent(Actor& parent);
	void ReparentActor(Actor* childActorToReparent);

	void UpdateCameraPos();

	void MoveForward(float deltaTime);
	void MoveBackward(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void Jump();

	void CallMoveForward();
	void CallMoveBackward();
	void CallMoveLeft();
	void CallMoveRight();

	bool IsGrounded();

	void UpdateIsGrounded();

	/*------GetComponents------*/
	/**
	 *@brief Get a Transform in TransformSystem
	 *@return nullptr if the actor doesn't have a Transform
	 */
	Transform* GetTransform();
	/**
	 *@brief Get a Camera in CameraSystem
	 *@return nullptr if the actor doesn't have a Camera
	 */
	Camera* GetCameraComponent();
	/**
	 *@brief Get a Light in LightSystem
	 *@return nullptr if the actor doesn't have a Light
	 */
	template<typename T>
	[[nodiscard]] T* GetLightComponent();
	/**
	 *@brief Get a Collider in PhysixSystem
	 *@return nullptr if the actor  doesn't have a Collider
	 */
	Collider* GetPhysicsComponent();
	
	void GetAllScriptIndex();
	std::vector<ScriptComponent>::iterator FindSpecificScript(size_t entityId, int idScript);
	/**
	 *@brief Get a Sound in SoundManager
	 *@return nullptr if the actor  doesn't have a Sound
	 */
	SoundComponent* GetSoundComponent();
	/**
	 *@brief Get a MeshComponent in StaticMeshSystem
	 *@return nullptr if the actor  doesn't have a Static mesh 
	 */
	StaticMesh* GetMeshComponent();
	
	bool CheckIfTransformChange();


	Entity m_idEntity = NULL;
	std::vector<Actor*> m_children;


	std::string m_name;
	//std::string m_actorNameInput;
	char m_actorNameInput[2048] = "";

	size_t m_indexInMyParent = SIZE_MAX;

	float m_deltaTime = 0.f;

	bool m_bIsController = false;
	bool m_bIsOG = false;

	/**
	 *@brief Component Transform Index
	 *
	 */
	size_t m_indexTransform = SIZE_MAX;


	bool m_bIsPossessed = false;
	bool m_bIsGrounded = true;
protected:
	Actor* m_parent = nullptr;
	std::vector<size_t> m_indexesScript;
private:
	/**
	 * @brief World Instance
	 */
	World* m_world = nullptr;
	
	/*-------Pointer in Systems*/
	TransformSystem* m_tSystem = nullptr;
	CameraSystem* m_camSystem = nullptr;
	LightSystem* m_lightSystem = nullptr;
	PhysixSystem* m_phySystem = nullptr;
	ScriptingSystem* m_scriptSystem = nullptr;
	SoundSystem* m_soundSystem = nullptr;
	StaticMeshSystem* m_staticMeshSystem = nullptr;
		
	bool m_bInJump = false;


	/**
	 *@brief Remove All components in Actor
	 * 
	 */
	void RemoveAllComponents();

	
};

template <typename T>
void Actor::AddComponent(size_t indexSystem)
{
	World::GetInstance().AddComponentID<T>(m_idEntity, indexSystem);
}

template<typename T>
inline size_t Actor::GetComponent()
{
	return World::GetInstance().GetComponentID<T>(m_idEntity);
}

template<typename T>
inline T* Actor::GetLightComponent()
{
	const size_t indexLight = GetComponent<LightComponent>();
	if (indexLight != INVALID_INDEX)
	{
		size_t idLight = World::GetInstance().GetComponentID<LightComponent>(m_idEntity);
		return m_lightSystem->GetLight<T>(idLight);
	}
	return nullptr;
}
