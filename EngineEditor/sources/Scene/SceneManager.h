#pragma once
#include <stdexcept>
#include <unordered_map>

#include "Scene.h"

#include <vector>

#include "../Components/Transform.h"

class SceneManager
{
public:
	~SceneManager();
	SceneManager(SceneManager const&) = delete;
	SceneManager& operator=(SceneManager const&) = delete;

	static SceneManager& GetInstance();

	void Update(float dT)
	{
		if (!m_scenes.empty() && m_currScene != -1)
			m_scenes[m_currScene].Update(dT);
	}

	static void NewScene();
	static void AddScene(Scene& newScene, bool newID = false);
	static void DeleteScene(size_t sceneID);
	static void DeleteDuplicateScene(size_t index);
	static void ChangeScene(size_t sceneID, bool revertTrans = false);
	static void DuplicateCurrScene();
	static void ChangeSceneAt(size_t sceneID, Scene& newScene);
	static void AddScenePath(std::string sceneName, std::string path);
	static void LoadKnownScene(std::string sceneName);
	static void LoadKnownSceneOnUpdate(std::string sceneName);

	void RemovePossessedActor(Actor* actor);

	void SetActorPossess(int id);

	std::unordered_map<std::string, std::string>& GetKnownScenes();
	void SetKnownScenes(std::vector<std::pair<std::string, std::string>>& levels);

	size_t GetScenesSize() { return m_scenes.size(); }
	size_t GetCurrentID() { return m_currScene; }
	Scene* GetScene();
	Scene* GetSceneAt(size_t index)
	{
		if (index <= m_scenes.size() - 1)
			return &m_scenes[index];
		else
			throw std::runtime_error("no scene exists with this ID");
	}
	float m_indexUpdate = 0;
private:
	SceneManager()
	{
		m_scenes.reserve(5);
	}
	void DeleteAllChildren(Actor* actor);
	void DeleteSceneMgr(Actor* actor);
	void RevertTransRecursive(Actor& actor);
	void DuplicateChildrenRecursive(Actor& actor);
	size_t m_currScene = static_cast<size_t>(-1);
	std::vector<Scene> m_scenes;
	std::vector<size_t> m_duplicatedScenes;
	std::vector<std::pair<Transform, size_t>> m_transforms;
	std::unordered_map<size_t, Transform> m_savedTrans;
	std::unordered_map<std::string, std::string> m_scenesKnown;
};
