#include "CameraControlSystem.h"

#include <iostream>


#include "../../../../InputSystem/DefineInput.h"
#include "../../../../InputSystem/InputManager.h"
#include "../../../../Components/Camera.h"
#include "../../../../ECS/World.h"
#include "../../../../Scene/Actor.h"
#include "../../../../Scene/Scene.h"
#include "../../../../Systems/RenderSystem.h"
#include "../../../../Systems/CameraSystem.h"
#include "Raycast/Raycast.h"
#include "Window/Window.h"
#include "../../libgfx/sources/RenderingManager.h"

void CameraControlSystem::Init(InputManager* inputManager, Window* window, Scene* scene)
{
	ImVec2 test;
	m_world = &World::GetInstance();

	m_window = window;
	if(m_bisFirstTime)
	{
		Camera* camera = scene->m_root->GetCameraComponent();
		if (camera)
		{
			m_cameraEditor = camera;
		}
		m_bisFirstTime = false;
	}
	else
	{
		window->SetCursorMode(true);
		bfreeCam = false;
	}

	if (inputManager)
	{
		inputManager->SaveKey("MoveFront", KEY_W);
		inputManager->SaveKey("MoveLeft", KEY_A);
		inputManager->SaveKey("MoveBack", KEY_S);
		inputManager->SaveKey("MoveRight", KEY_D);
		inputManager->SaveKey("MoveUp", KEY_Q);
		inputManager->SaveKey("MoveDown", KEY_E);

		inputManager->SaveMouseKey("ActiveFreeCam", MOUSE_BUTTON_RIGHT);
		inputManager->BindMouseKey("ActiveFreeCam", KeyEvent::PRESS, this, &CameraControlSystem::ActiveFreeCamera);
		
		inputManager->SaveMouseKey("Pick", MOUSE_BUTTON_LEFT);
		inputManager->BindMouseKey("Pick", KeyEvent::RELEASE, this, &CameraControlSystem::PickUp);
		

		inputManager->BindKey("MoveFront", KeyEvent::REPEAT, this, &CameraControlSystem::MoveFront);
		inputManager->BindKey("MoveLeft", KeyEvent::REPEAT, this, &CameraControlSystem::MoveLeft);
		inputManager->BindKey("MoveBack", KeyEvent::REPEAT, this, &CameraControlSystem::MoveBack);
		inputManager->BindKey("MoveRight", KeyEvent::REPEAT, this, &CameraControlSystem::MoveRight);
		inputManager->BindKey("MoveUp", KeyEvent::REPEAT, this, &CameraControlSystem::MoveUp);
		inputManager->BindKey("MoveDown", KeyEvent::REPEAT, this, &CameraControlSystem::MoveDown);

		if (window)
		{
			window->SetCursorMode(true);
			bfreeCam = false;
		}
		inputManager->SetMousePosCallback(this, &CameraControlSystem::MouseMoveCamera);
		
	}
}

void CameraControlSystem::Update(float deltaTime)
{
	m_deltaTime = deltaTime;
}

void CameraControlSystem::MoveFront()
{
	if (bfreeCam)
	{
		m_cameraEditor->MoveCamera(MovCamera::FORWARD, m_deltaTime);
	}
}

void CameraControlSystem::MoveBack()
{
	if (bfreeCam)
	{
		m_cameraEditor->MoveCamera(MovCamera::BACKWARD, m_deltaTime);
	}
}

void CameraControlSystem::MoveLeft()
{
	if (bfreeCam)
	{
		m_cameraEditor->MoveCamera(MovCamera::LEFT, m_deltaTime);
	}
}

void CameraControlSystem::MoveRight()
{
	if (bfreeCam)
	{
		m_cameraEditor->MoveCamera(MovCamera::RIGHT, m_deltaTime);
	}
}

void CameraControlSystem::MoveUp()
{
	if (bfreeCam)
	{
		m_cameraEditor->MoveCamera(MovCamera::UP, m_deltaTime);
	}
}

void CameraControlSystem::MoveDown()
{
	if (bfreeCam)
	{
		m_cameraEditor->MoveCamera(MovCamera::DOWN, m_deltaTime);
	}
}

void CameraControlSystem::MouseMoveCamera(double xpos, double ypos)
{
	m_mousePosX = xpos;
	m_mousePosY = ypos;
	if(bfreeCam)
	{
		if (m_bFirstTime)
		{
			m_lastX = xpos;
			m_lasty = ypos;
			m_bFirstTime = false;
		}
		double xoffset = xpos - m_lastX;
		double yoffset = m_lasty - ypos;

		m_lastX = xpos;
		m_lasty = ypos;
		m_cameraEditor->MouseMoveCamera((float)xoffset, (float)yoffset);
	}
}


void CameraControlSystem::ActiveFreeCamera()
{
	
	auto& windowmanager = World::GetInstance().GetSystem<RenderSystem>()->m_renderingMgr->m_UIWindowManager;


	if ((m_mousePosX >= windowmanager.GetViewportSceneTopLeftPos().x && m_mousePosX <= windowmanager.GetViewportSceneBottomRightPos().x) &&
		(m_mousePosY >= windowmanager.GetViewportSceneTopLeftPos().y && m_mousePosY <= windowmanager.GetViewportSceneBottomRightPos().y) && bfreeCam == false)
	{
		bfreeCam = true;
		m_bFirstTime = true;
		//bfreeCam = true;
		m_window->SetCursorMode(!bfreeCam);
	}
	else
	{
		bfreeCam = false;
		//bfreeCam = true;
		m_window->SetCursorMode(!bfreeCam);
	}

	
	
}

void CameraControlSystem::PickUp()
{
	//HitInfo info;
	//if(Raycast::Ray({ m_mousePosX, m_mousePosY, m_cameraEditor->GetCameraPos().z }, m_cameraEditor->m_cameraFront, 100000, info))
	//{
	//	//info.GetIndexCollider();
	//	std::cout << "I touch an Character" << std::endl;
	//}
	//bfreeCam = false;
	//m_window->SetCursorMode(!bfreeCam);
}
