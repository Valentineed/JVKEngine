#pragma once
#include "LightSystem.h"
#include "../ECS/System.h"
#include "StaticMeshSystem.h"
#include "../Resources/ComponentsDataManager.h"
#include "UI/UIWindows/UIWindowBuild.h"
#include "UI/UIWindows/UIWindowComponents.h"
#include "UI/UIWindows/UIWindowFIleBrowser.h"
#include "UI/UIWindows/UIWindowMainToolBar.h"
#include "UI/UIWindows/UIWindowViewportScene.h"
#include "UI/UIWindows/UIWindowWorldHierarchy.h"
#include "UI/UIWindows/UIWindowUIInGamePreview.h"
class SoundManager;
class FSM;
enum class STATE;
struct Camera;
class InputManager;
#include "../Scene/Actor.h"

class InputManager;
class World;
class Scene;
class InputManager;
class Window;
struct RenderingMgr;

class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();

	void Init(Window* window, size_t sceneID, RenderingMgr* render, bool aliasing);
	void Update(float deltaTime, Window* window, size_t sceneID, STATE state, FSM* fsm);
	void UpdateMainCam(STATE currState, float deltaTime);
	void End();
	void LoadTexturedModel(std::string_view obj, std::string_view text);
	size_t LoadModel(std::string_view obj);
	size_t LoadTexture(std::string_view path);

	void CreateUIWindows();

	void UpdateDataFromUI(size_t sceneID, FSM* fsm, Window* window);
	void UpdateDataFromBuildWindow();
	void UpdateDataFromComponentsWindow();
	void UpdateDataFromFileBrowserWindow();
	void UpdateDataFromUIPreviewWindow();

	void UpdateChangingStateFromUI(FSM* fsm, Window* window);

	void UpdateReparentingActors(Scene* scene);

	UIWindowUIInGamePreview& GetUIWindow();

	UIWindowViewportScene& GetViewportWindow();
	
	Actor& GetCamera() { return m_mainCamera; }
	Camera* m_currCam = nullptr;
	RenderingMgr* m_renderingMgr;
	TransformSystem* m_transformSystem;
private:
	std::shared_ptr<StaticMeshSystem> m_staticMeshSystem;
	std::shared_ptr<LightSystem>m_lightSystem;
	Actor m_mainCamera;
	World* m_world;

	/*Camera old pos for calculate listenner in audio*/
	Vector3 m_camOldPos;
	SoundManager* m_soundManager = nullptr;
	
	UIWindowFIleBrowser UIWindowFIleBrowser;
	UIWindowWorldHierarchy UIWindowWorldHierarchy;
	UIWindowComponents UIWindowComponents;
	UIWindowViewportScene UIWindowViewportScene;
	UIWindowUIInGamePreview UIWindowUIInGamePreview;
	UIWindowBuild UIWindowBuild;
	UIWindowMainToolBar UIWindowMainToolBar;

	ComponentsDataManager m_componentsDataManager;
protected:
		static  const  size_t s_invalidIndex = SIZE_MAX;
};
