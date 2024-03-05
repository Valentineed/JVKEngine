#include "Actor.h"

#include <iostream>

#include "../Components/Transform.h"
#include "../ECS/World.h"
#include "../Systems/TransformSystem.h"

#include <string>

#include "Defines.h"
#include "../Components/Camera.h"
#include "../Components/SoundComponent.h"
#include "../Components/Lights/LightComponent.h"
#include "../Systems/CameraSystem.h"
#include "../Systems/LightSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/ScriptingSystem.h"
#include "../Systems/SoundSystem.h"
#include "Forces/ForceMode.h"
#include "Raycast/Raycast.h"
Actor::Actor(const std::string& name)
{
	m_idEntity = World::GetInstance().CreateEntity();

	m_world = &World::GetInstance();
	/*---------Set All System pointer--------*/
	m_tSystem = m_world->GetSystem<TransformSystem>();
	m_camSystem = m_world->GetSystem<CameraSystem>();
	m_lightSystem = m_world->GetSystem<LightSystem>();
	m_phySystem = m_world->GetSystem<PhysixSystem>();
	m_scriptSystem = m_world->GetSystem<ScriptingSystem>();
	m_soundSystem = m_world->GetSystem<SoundSystem>();
	m_staticMeshSystem = m_world->GetSystem<StaticMeshSystem>();

	/*Create a Transform and attach to actor*/
	m_indexTransform = m_tSystem->Register();
	AddComponent<Transform>(m_indexTransform);

	/*Set Default Name*/
	if (name == "Actor")
	{
		m_name.append(name.c_str());
		m_name += std::to_string(m_idEntity);
	}
	else
	{
		m_name.append(name.c_str());
	}

	strcpy_s(m_actorNameInput, m_name.data());
	//m_actorNameInput = m_name;
	m_bIsOG = true;
}

void Actor::AddChild(Actor* actor)
{
	if (actor != nullptr)
	{
		size_t index = m_children.size();
		m_children.push_back(actor);
		actor->m_parent = this;
		actor->m_indexInMyParent = index;
		m_tSystem->AddChild(m_indexTransform, actor->m_indexTransform);
	}
}

/*--------Delete The Actor----------*/
bool Actor::DeleteThis(bool scene)
{
	if (m_children.empty())
	{
		if (!scene)
		{
			RemoveAllComponents();
		}
		World::GetInstance().DestroyEntity(m_idEntity);
		delete this;
		return  true;
	}
	return false;
}

bool Actor::Remove()
{
	if (m_children.empty())
	{
		if (m_parent != nullptr)
		{
			m_parent->RemoveChild(m_indexInMyParent);
			//delete this;
			return true;
		}
		World::GetInstance().GetSystem<TransformSystem>()->CleanUpSystem();
		return true;
	}
	return false;
}

void Actor::RemoveLastChild()
{
	m_children.back()->~Actor();
	m_children.back()->DeleteThis();
	m_children.pop_back();
}

void Actor::PopLastChild()
{
	m_children.pop_back();
}

void Actor::RemoveAllComponents()
{
	auto* world = &World::GetInstance();
	size_t invalid = -1;

	size_t id = world->GetComponentID<StaticMesh>(m_idEntity);
	if (id != invalid)
	{
		world->GetSystem<StaticMeshSystem>()->RemoveStaticMesh(id, m_idEntity);
	}

	id = world->GetComponentID<Collider>(m_idEntity);
	if (id != invalid)
	{
		world->GetSystem<PhysixSystem>()->RemoveComponent(id, m_idEntity);
	}

	id = world->GetComponentID<Camera>(m_idEntity);
	if (id != invalid)
	{
		world->GetSystem<CameraSystem>()->RemoveCamera(id, m_idEntity);
	}

	id = world->GetComponentID<SoundComponent>(m_idEntity);
	if (id != invalid)
	{
		world->GetSystem<SoundSystem>()->RemoveSound(id, m_idEntity);
	}

	id = world->GetComponentID<LightComponent>(m_idEntity);
	if (id != invalid)
	{
		world->GetSystem<LightSystem>()->RemoveLight(id, m_idEntity);
	}
}

void Actor::RemoveChild(size_t index)
{
	/*Remove all component of child*/
	if (index == m_children.size() - 1)
	{
		World::GetInstance().GetSystem<TransformSystem>()->RemoveChild(m_indexTransform, m_children[index]->m_indexTransform, true);

		RemoveLastChild();
	}
	else
	{
		World::GetInstance().GetSystem<TransformSystem>()->RemoveChild(m_indexTransform, m_children[index]->m_indexTransform, false);
		m_children.back()->m_indexInMyParent = m_children[index]->m_indexInMyParent;

		std::iter_swap(m_children.begin() + index, m_children.end() - 1);
		RemoveLastChild();
	}
}

void Actor::ReparentActor(Actor* childActorToReparent)
{
	if (childActorToReparent->m_parent == this)
	{
		std::cout << "Already Child of this parent";
		return;
	}

	if (childActorToReparent == this->m_parent)
	{
		std::cout << "Cant set the parent into the child";
		return;
	}

	bool IsChildActorLastOFChildren = false;
	std::cout << childActorToReparent->m_name << " Moved into ";
	std::cout << m_name << std::endl;

	for (auto it = childActorToReparent->m_parent->m_children.begin(); it != childActorToReparent->m_parent->m_children.end(); ++it)
	{
		if (std::next(it) == childActorToReparent->m_parent->m_children.end()) // last element
		{
			// do something else
			if (childActorToReparent == static_cast<Actor*>(*it))
			{
				childActorToReparent->m_parent->PopLastChild();
				AddChild(childActorToReparent);
				IsChildActorLastOFChildren = true;
				break;
			}
		}
	}

	if (IsChildActorLastOFChildren == false)
	{
		auto iteratorOfChildrenActor = std::find(childActorToReparent->m_parent->m_children.begin(), childActorToReparent->m_parent->m_children.end(), childActorToReparent);

		for (auto it = childActorToReparent->m_parent->m_children.begin(); it != childActorToReparent->m_parent->m_children.end(); ++it)
		{
			if (it == iteratorOfChildrenActor) // last element
			{
				std::iter_swap(iteratorOfChildrenActor, childActorToReparent->m_parent->m_children.end() - 1);
				childActorToReparent->m_parent->PopLastChild();
				AddChild(childActorToReparent);
				break;
			}
		}
	}
}
/*---------------------------------*/

void Actor::SetParent(Actor& parent)
{
	m_parent = &parent;
}

/*--------STATE FUNCTIONS----------*/
void Actor::BeginPlay()
{
	GetAllScriptIndex();

	if (m_indexesScript.empty() == false)
	{
		for (size_t index : m_indexesScript)
		{
			World::GetInstance().GetSystem<ScriptingSystem>()->m_scripts[index].Start();
		}
	}
	
	if (GetSoundComponent() != nullptr)
	{
		auto* mysound = World::GetInstance().GetSystem<SoundSystem>()->GetSound(World::GetInstance().GetComponentID<SoundComponent>(m_idEntity));
		if (mysound->m_bPlayOnWake == true)
		{
			mysound->Active3DLinear();
			mysound->Play();
			mysound->SetMute(mysound->m_bIsMute);
			mysound->SetVolume(mysound->m_volume);
			//mysound->SetFrequency(1.f);
			mysound->SetPosition(GetTransform()->GetWorldLocation(), 0);
		}
	}
	auto id = m_idEntity;

	

	for (int index = m_children.size() - 1; index >= 0; index--)
	{
		m_children[index]->BeginPlay();
	}
}

void Actor::Begin()
{
	for (auto& actor : m_children)
	{
		actor->Begin();
	}
}

void Actor::Update(float deltaTime)
{
	m_deltaTime = deltaTime;
	if(m_bIsController)
	{
		UpdateIsGrounded();
	}

	if (m_indexesScript.empty() == false)
	{
		for (size_t index : m_indexesScript)
		{
			World::GetInstance().GetSystem<ScriptingSystem>()->m_scripts[index].Update();
		}
	}

	if (GetSoundComponent() != nullptr)
	{
		auto mysound = World::GetInstance().GetSystem<SoundSystem>()->GetSound(World::GetInstance().GetComponentID<SoundComponent>(m_idEntity));
		mysound->SetPosition(GetTransform()->GetWorldLocation(), deltaTime);
	}
	
	for (int index = m_children.size() -1; index >= 0; index--)
	{
		m_children[index]->Update(deltaTime);
		if(m_children.size() == 0)
		{
			break;
		}
	}
}

void Actor::End()
{
	m_indexesScript.clear();
	if (GetSoundComponent() != nullptr)
	{
		auto mysound = World::GetInstance().GetSystem<SoundSystem>()->GetSound(World::GetInstance().GetComponentID<SoundComponent>(m_idEntity));
		mysound->Stop();
	}
	
	for (int index = m_children.size() -1; index >= 0; index--)
	{
		m_children[index]->End();
	}
}
/*------------------------------*/

void Actor::SetTransform(Transform& trans)
{
	if (m_tSystem == nullptr)
		m_tSystem = World::GetInstance().GetSystem<TransformSystem>();
	m_indexTransform = GetTransform()->myIndexSystem;
	GetTransform()->SetLocalTransformMat(trans.m_localTransform);
}

Actor* Actor::GetParent()
{
	return m_parent;
}

void Actor::UpdateCameraPos()
{
	if (!m_camSystem)
	{
		m_camSystem = m_world->GetSystem<CameraSystem>();
	}

	const size_t indexCam = GetComponent<Camera>();

	if (indexCam != INVALID_INDEX)
	{
		m_camSystem->GetCamera(indexCam)->SetCameraPos(GetTransform()->GetWorldLocation());
	}
}

void Actor::MoveForward(float deltaTime)
{
	const float velocity =  3.f * deltaTime;
	//ForceMode force(FORCE);
	const Vector3 camFront = GetCameraComponent()->m_cameraFront;
	const Vector3 front = { camFront.x, 0 ,camFront.z };
	auto pos = GetTransform()->GetWorldLocation();

	GetPhysicsComponent()->ResetVelocity(true, false, true);
	GetTransform()->SetLocalPosition(GetTransform()->GetWorldLocation() + front * velocity);
}

void Actor::MoveBackward(float deltaTime)
{
	float velocity = 3.f * deltaTime;

	Vector3 camFront = GetCameraComponent()->m_cameraFront;
	Vector3 front = { camFront.x, 0 ,camFront.z };
	GetPhysicsComponent()->ResetVelocity(true, false, true);
	GetTransform()->SetLocalPosition(GetTransform()->GetWorldLocation() - (front * velocity));
}

void Actor::MoveLeft(float deltaTime)
{
	float velocity = 3.f * deltaTime;
	
	Vector3 camRight = GetCameraComponent()->m_cameraRight;
	Vector3 right = { camRight.x, 0 ,camRight.z };
	GetPhysicsComponent()->ResetVelocity(true, false, true);
	GetTransform()->SetLocalPosition(GetTransform()->GetWorldLocation() + right * velocity);
}

void Actor::MoveRight(float deltaTime)
{
	float velocity = 3.f * deltaTime;

	Vector3 camRight = GetCameraComponent()->m_cameraRight;
	Vector3 right = { camRight.x, 0 ,camRight.z };
	GetPhysicsComponent()->ResetVelocity(true, false, true);
	GetTransform()->SetLocalPosition(GetTransform()->GetWorldLocation() - right * velocity);
}

void Actor::Jump()
{
	ForceMode force(FORCE);
	if (GetPhysicsComponent() != nullptr && m_bIsGrounded)
	{
		GetPhysicsComponent()->AddForce({ 0,30000,0 }, force);
		m_bInJump = true;
	}
}

void Actor::CallMoveForward()
{
	MoveForward(m_deltaTime);
}

void Actor::CallMoveBackward()
{
	MoveBackward(m_deltaTime);
}

void Actor::CallMoveLeft()
{
	MoveLeft(m_deltaTime);
}

void Actor::CallMoveRight()
{
	MoveRight(m_deltaTime);
}

bool Actor::IsGrounded()
{
	return m_bIsGrounded;
}

void Actor::UpdateIsGrounded()
{
	HitInfo info;
	if(m_bInJump)
	{
		if (GetPhysicsComponent() != nullptr)
		{
			//std::cout << GetPhysicsComponent()->GetLinearVelocity().y << std::endl;
			const auto pos = GetTransform()->GetWorldLocation();
			bool ray = Raycast::Ray({ pos.x, pos.y - 1.501f, pos.z }, { 0.f, -1.f, 0.f }, 0.01f, info);
			if (ray)
			{
				if (info.GetEntityID() != m_idEntity)
				{
					std::cout << "Ray LOL" << std::endl;
					m_bIsGrounded = true;
					m_bInJump = false;
				}
			}
			else
			{
				std::cout << "Nothing" << std::endl;
				m_bIsGrounded = false;
			}
		}
	}
}


bool Actor::CheckIfTransformChange()
{
	if (!m_tSystem)
	{
		m_tSystem = m_world->GetSystem<TransformSystem>();
	}
	if (m_indexTransform == INVALID_INDEX)
	{
		throw std::runtime_error("Missing Transform index in actor!");
	}
	const auto iterator = std::find(m_tSystem->modifyTindex.begin(), m_tSystem->modifyTindex.end(), m_indexTransform);
	if(iterator == m_tSystem->modifyTindex.end())
	{
		return false;
	}
	return true;
}

/*---------GET COMPONENTS----------*/
Transform* Actor::GetTransform()
{
	m_tSystem = m_world->GetSystem<TransformSystem>();
	
	if (m_indexTransform == INVALID_INDEX)
	{
		return nullptr;
	}

	return  m_tSystem->GetTransform(m_indexTransform);
}

Camera* Actor::GetCameraComponent()
{
	if (!m_camSystem)
	{
		m_camSystem = m_world->GetSystem<CameraSystem>();
	}
	const size_t cameraComponentIdFound = GetComponent<Camera>();
	if (cameraComponentIdFound != INVALID_INDEX)
	{
		return m_camSystem->GetCamera(cameraComponentIdFound);
	}
	return nullptr;
}

StaticMesh* Actor::GetMeshComponent()
{
	if (!m_staticMeshSystem)
	{
		m_staticMeshSystem = m_world->GetSystem<StaticMeshSystem>();
	}
	const size_t meshComponentIdFound = GetComponent<StaticMesh>();

	if (meshComponentIdFound != INVALID_INDEX)
	{
		return m_staticMeshSystem->GetStaticMesh(meshComponentIdFound);
	}
	return nullptr;
}

Collider* Actor::GetPhysicsComponent()
{
	if (!m_phySystem)
	{
		m_phySystem = m_world->GetSystem<PhysixSystem>();
	}
	const size_t physicsComponentIdFound = GetComponent<Collider>();

	if (physicsComponentIdFound != INVALID_INDEX)
	{
		return m_phySystem->GetCollider(physicsComponentIdFound);
	}
	return nullptr;
}

SoundComponent* Actor::GetSoundComponent()
{
	if (!m_soundSystem)
	{
		m_soundSystem = m_world->GetSystem<SoundSystem>();
	}
	const size_t soundComponentIdFound = GetComponent<SoundComponent>();

	if (soundComponentIdFound != INVALID_INDEX)
	{
		return m_soundSystem->GetSound(soundComponentIdFound);
	}
	return nullptr;
}

void Actor::GetAllScriptIndex()
{
	if (!m_scriptSystem)
	{
		m_scriptSystem = m_world->GetSystem<ScriptingSystem>();
	}
	auto it = FindSpecificScript(m_idEntity, 0);

	while (it != m_scriptSystem->m_scripts.end())
	{
		size_t index = std::distance(m_scriptSystem->m_scripts.begin(), it);

		if (it != m_scriptSystem->m_scripts.end())
		{
			m_indexesScript.push_back(index);
		}
		it = FindSpecificScript(m_idEntity, index + 1ull);
	}
}

std::vector<ScriptComponent>::iterator Actor::FindSpecificScript(size_t entityID, int idScript)
{
	auto& Scripts = World::GetInstance().GetSystem<ScriptingSystem>()->m_scripts;
	auto test =  std::find_if(Scripts.begin() + idScript, Scripts.end(), [entityID](ScriptComponent& obj) { return obj.GetEntityID() == entityID; });

	return test;
}
/*---------------------------------*/