#pragma once
#include "../../../../ECS/System.h"
#include "Vector/TVector2.h"

struct Camera;
class Window;
class InputManager;
class Scene;
class World;

class CameraControlSystem : public System
{
public:
	void Init(InputManager* inputManager, Window* window, Scene* world);

	void Update(float deltaTime);
private:
	float m_deltaTime = 0.f;
	Camera* m_cameraEditor = nullptr;
	Window* m_window = nullptr;
	LibMath::Vector2 m_sizeWindow;
	void MoveDown();
	void MouseMoveCamera(double xpos, double ypos);
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveFront();
	void ActiveFreeCamera();
	void PickUp();

	bool m_bFirstTime = true;
	double m_lastX = 0.0f;
	double m_lasty = 0.0f;
	World* m_world = nullptr;
	bool m_bisFirstTime = true;

	float m_mousePosX;
	float m_mousePosY;
	
	bool bfreeCam = false;
};
