#include "SceneManager.h"
#include "Actor.h"
#include "../Systems/TransformSystem.h"
#include "../Systems/PhysixSystem.h"

#include <iostream>

#include "../Serialization/Serializer.h"

SceneManager::~SceneManager()
{
	SceneManager& sm = GetInstance();
	if (!sm.m_scenes.empty())
	{
		for (size_t sceneID = 0; sceneID < sm.m_scenes.size(); sceneID++)
		{
			for (auto id : sm.m_duplicatedScenes)
			{
				if (sceneID == id && &sm.m_scenes[sceneID] == &sm.m_scenes[id])
				{
					DeleteDuplicateScene(sceneID);
				}
			}
		}

		for (auto scene : sm.m_scenes)
		{
			DeleteSceneMgr(scene.m_root);
		}
	}
}

void SceneManager::DeleteAllChildren(Actor* actor)
{
	if (actor == nullptr)
	{
		return;
	}
	if (!actor->m_children.empty())
	{
		for (int i = actor->m_children.size() - 1; i > -1; i--)
		{
			DeleteAllChildren(actor->m_children[i]);
		}
		actor->m_children.clear();
	}
	if (!actor->Remove())
	{
		std::cerr << "One Actor is not Delete!!!" << std::endl;
	}
}

void SceneManager::DeleteSceneMgr(Actor* actor)
{
	if (actor == nullptr)
	{
		return;
	}
	if (!actor->m_children.empty())
	{
		for (auto child : actor->m_children)
		{
			DeleteSceneMgr(child);
		}
		actor->m_children.clear();
	}
	if (!actor->DeleteThis(true))
	{
		std::cerr << "One Actor is not Delete!!!" << std::endl;
	}
}

SceneManager& SceneManager::GetInstance()
{
	static SceneManager newInstance;

	return newInstance;
}

void SceneManager::NewScene()
{
	SceneManager& sm = GetInstance();

	sm.m_scenes.emplace_back();
	sm.m_currScene = sm.m_scenes.size() - 1;
	sm.GetScene()->Begin();
}

void SceneManager::AddScene(Scene& newScene, bool newID)
{
	SceneManager& sm = GetInstance();

	sm.m_scenes.push_back(newScene);
	if (sm.m_currScene == -1 && newID)
	{
		sm.m_currScene = sm.m_scenes.size() - 1;
	}
}

void SceneManager::DeleteScene(size_t sceneID)
{
	SceneManager& sm = GetInstance();

	sm.DeleteAllChildren(sm.m_scenes[sceneID].m_root);
	sm.m_scenes.erase(sm.m_scenes.begin() + sceneID);
}

void SceneManager::DeleteDuplicateScene(size_t index)
{
	SceneManager& sm = GetInstance();
	if (sm.m_scenes.size() > index)
	{
		for (size_t i = 0; i < sm.m_duplicatedScenes.size(); i++)
		{
			if (index == sm.m_duplicatedScenes[i])
				sm.m_duplicatedScenes.erase(sm.m_duplicatedScenes.begin() + i);
		}
		sm.m_scenes.erase(sm.m_scenes.begin() + index);
	}
}

void SceneManager::ChangeScene(size_t sceneID, bool revertTrans)
{
	SceneManager& sm = GetInstance();

	if (sm.m_scenes.size() > sceneID)
	{
		sm.m_scenes[sm.m_currScene].End();
		sm.m_currScene = sceneID;
		if (revertTrans)
		{
			sm.RevertTransRecursive(*sm.GetSceneAt(sm.m_currScene)->m_root);
		}
		sm.m_scenes[sm.m_currScene].Begin();
	}
}

void SceneManager::RevertTransRecursive(Actor& actor)
{
	if (actor.m_children.empty())
		return;

	SceneManager& sm = GetInstance();
	World& world = World::GetInstance();
	size_t id;

	for (int i = 0; i < actor.m_children.size(); i++)
	{
		id = world.GetComponentID<Transform>(actor.m_children[i]->m_idEntity);

		*world.GetSystem<TransformSystem>()->GetTransform(id) = sm.m_savedTrans.at(id);

		sm.RevertTransRecursive(*actor.m_children[i]);
	}
	sm.m_savedTrans.clear();
}

void SceneManager::DuplicateCurrScene()
{
	SceneManager& sm = GetInstance();

	Scene newScene(*sm.GetSceneAt(sm.m_currScene));
	sm.m_savedTrans.reserve(sm.m_scenes.back().m_root->m_children.capacity());

	sm.DuplicateChildrenRecursive(*sm.GetSceneAt(sm.m_currScene)->m_root);

	sm.m_scenes.push_back(newScene);
	sm.m_currScene = sm.m_scenes.size() - 1;
	sm.m_duplicatedScenes.push_back(sm.m_currScene);
}

void SceneManager::DuplicateChildrenRecursive(Actor& actor)
{
	if (actor.m_children.empty())
		return;

	SceneManager& sm = GetInstance();
	World& world = World::GetInstance();
	size_t id;

	for (int i = 0; i < actor.m_children.size(); i++)
	{
		id = world.GetComponentID<Transform>(actor.m_children[i]->m_idEntity);
		sm.m_savedTrans.insert(std::make_pair(id, *world.GetSystem<TransformSystem>()->GetTransform(id)));

		DuplicateChildrenRecursive(*actor.m_children[i]);
	}
}

void SceneManager::ChangeSceneAt(size_t sceneID, Scene& newScene)
{
	SceneManager& sm = GetInstance();

	sm.m_scenes[sceneID] = newScene;
}

void SceneManager::AddScenePath(std::string sceneName, std::string path)
{
	SceneManager& sm = GetInstance();

	sm.m_scenesKnown.emplace(std::make_pair(sceneName, path));
}

void SceneManager::LoadKnownScene(std::string sceneName)
{
	SceneManager& sm = GetInstance();

	StatePlay* stateplay = sm.GetScene()->m_statePlay;
	
	sm.DeleteScene(sm.GetCurrentID());
	sm.NewScene();
	Scene scene = XML::Serializer::GetInstance().LoadScene(sceneName.c_str());
	sm.ChangeSceneAt(sm.GetInstance().GetCurrentID(), scene);

	sm.GetScene()->InitStatePlay(stateplay);

	World::GetInstance().GetSystem<TransformSystem>()->Update();
}

void SceneManager::LoadKnownSceneOnUpdate(std::string sceneName)
{
	SceneManager& sm = GetInstance();
	LoadKnownScene(sceneName);
	sm.m_indexUpdate++;
}

void SceneManager::RemovePossessedActor(Actor* actor)
{
	for (Actor* theactor : actor->m_children)
	{
		theactor->m_bIsPossessed = false;

		if (theactor->m_children.empty() == false)
		{
			RemovePossessedActor(theactor);
		}
	}
}

void SceneManager::SetActorPossess(int id)
{
	GetScene()->GetActor(GetScene()->m_root, id)->m_bIsPossessed = true;
}


std::unordered_map<std::string, std::string>& SceneManager::GetKnownScenes()
{
	return m_scenesKnown;
}	

void SceneManager::SetKnownScenes(std::vector<std::pair<std::string, std::string>>& levels)
{
	m_scenesKnown.clear();
	for (std::pair<std::string, std::string> pair : levels)
	{
		m_scenesKnown.insert({ pair.first, pair.second });
	}
}

Scene* SceneManager::GetScene()
{
	
	 return &m_scenes[m_currScene]; 
}
