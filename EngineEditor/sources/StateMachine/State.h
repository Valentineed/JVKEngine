#pragma once

class RenderingMgr;
class Scene;
class InputManager;
class Window;

enum class STATE
{
	ENGINE = 0,
	PLAY,
	NO_STATE //keep this entry last
};

class State 
{
public:
	State() = default;
	State(const State & other) = delete;
	State& operator=(const State && other) = delete;
	virtual ~State() = default;

	[[nodiscard]] virtual STATE GetState() const = 0;
	[[nodiscard]] virtual size_t GetSceneID() const = 0;
	virtual void Begin(Window* window, InputManager* inputManager, size_t sceneID, RenderingMgr* render) = 0;
	virtual STATE Update(Window* window, const float deltaTime) = 0;
	virtual void End() = 0;
};
