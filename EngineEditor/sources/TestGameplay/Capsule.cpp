#include "Capsule.h"

#include "Bullet.h"
#include "Actors/RigidType.h"
#include "../Components/Collider.h"
#include "../Components/Transform.h"
#include "../InputSystem/InputManager.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/CameraSystem.h"
#include "../Systems/ScriptingSystem.h"
#include "Shapes/Primitives/CapsuleShape.h"
#include "../InputSystem/DefineInput.h"
#include <filesystem>



#include "Bullet2.h"
#include "Button.h"
#include "../Systems/RenderSystem.h"
#include "RenderingManager.h"

Capsule::Capsule()
{
	m_name = "Capsule";
	m_bIsController = true;

	size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->RegisterDefaultShape(CapsuleShape{ 1.f, 0.5f }, RigidType::DYNAMIC,GetTransform(), m_idEntity);

	AddComponent<Collider>(indexCollider);
	GetPhysicsComponent()->LockRotation(true, true, true);
	GetPhysicsComponent()->SetMass(80);
	GetPhysicsComponent()->BindOnBeginContact(this, &Capsule::BeginOverlap);
	GetPhysicsComponent()->BindOnEndContact(this, &Capsule::EndOverlap);
	
	const size_t camIndex = World::GetInstance().GetSystem<CameraSystem>()->Register(GetTransform()->GetLocalLocation(), { 0.f, 0.f, -1.f }, { 0.f, 1.f, 0.f });

	AddComponent<Camera>(camIndex);

	World::GetInstance().GetSystem<ScriptingSystem>()->Register(m_idEntity, "Assets/Scripts/MyCharacterScript.lua");

	World::GetInstance().GetSystem<RenderSystem>()->m_renderingMgr->m_UIWindowManager.SetActorIDToPossess(m_idEntity);
}

Capsule::~Capsule()
{
	//delete m_shape;
	//delete m_collider;
}

void Capsule::Update(float deltaTime)
{
	Actor::Update(deltaTime);
	if(m_bIsPick && m_pickObject != nullptr)
	{
		auto offset = GetTransform()->GetWorldLocation();
		offset += GetCameraComponent()->m_cameraFront * 2;
		m_pickObject->GetTransform()->SetLocalPosition(offset);
		if(m_bfirstTime)
		{
			m_pickObject->GetPhysicsComponent()->LockRotation(true, true, true);
			m_bfirstTime = false;
		}
	}
	else if(!m_bIsPick && !m_bfirstTime)
	{
		m_bfirstTime = true;
	}
}

void Capsule::Begin()
{
	Actor::Begin();


}

void Capsule::BeginPlay()
{
	Actor::BeginPlay();
	auto inputManager = &InputManager::GetInstance();
	inputManager->SaveMouseKey("Fire", MOUSE_BUTTON_LEFT);
	inputManager->BindMouseKey("Fire", KeyEvent::PRESS, this, &Capsule::Fire);
	
	inputManager->SaveMouseKey("Fire2", MOUSE_BUTTON_RIGHT);
	inputManager->BindMouseKey("Fire2", KeyEvent::PRESS, this, &Capsule::Fire2);
	
	inputManager->SaveKey("Pick", KEY_E);
	inputManager->BindKey("Pick", KeyEvent::PRESS, this, &Capsule::PickObject);
}

void Capsule::End()
{
	Actor::End();
	
}

void Capsule::BeginOverlap(size_t indexOtherCollider)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	auto* otherActor = scene->GetActor(scene->m_root, indexOtherCollider);
	if (otherActor->m_name == "PickUpObject")
	{
		m_pickObject = otherActor;
	}
}

void Capsule::EndOverlap(size_t /*indexOtherCollider*/)
{
}

void Capsule::Fire()
{
	new Bullet(GetCameraComponent()->m_cameraFront *7.f,  GetTransform()->GetWorldLocation() + GetCameraComponent()->m_cameraFront *2.f , this);
}

void Capsule::Fire2()
{
	new Bullet2(GetCameraComponent()->m_cameraFront * 20.f, GetTransform()->GetWorldLocation() + GetCameraComponent()->m_cameraFront * 2.f, this);
}

void Capsule::PickObject()
{
	if(m_pickObject != nullptr)
	{
		m_bIsPick = !m_bIsPick;
		if(!m_bIsPick)
		{
			m_pickObject = nullptr;
		}
	}
	if(m_bOpenTrap)
	{
		if(m_button!= nullptr)
		{
			static_cast<Button*>(m_button)->DeleteTrap();
		}
	}
}
