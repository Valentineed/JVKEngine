#include "Scene.h"

#include <iostream>

#include "Actor.h"
#include "../InputSystem/DefineInput.h"
#include "../InputSystem/InputManager.h"

Scene::~Scene()
{	
	std::cout << "Destroyed" << std::endl;	
}

void Scene::Begin()
{
	if (m_root != nullptr)
	{
		m_root->Begin();
	}
}

void Scene::BeginPlay()
{
    if (m_root != nullptr)
    {
        m_root->BeginPlay();
    }
	
#ifndef _SHIPPING
    InputManager::GetInstance().SaveKey("ChangeState", KEY_ESCAPE);
    InputManager::GetInstance().BindKey("ChangeState", KeyEvent::PRESS, m_statePlay, &StatePlay::ChangeState);
#endif

    InputManager::GetInstance().SetMousePosCallback(m_statePlay, &StatePlay::MouseMoveCamera);
}

void Scene::Update(float deltaTime)
{
	if (m_root != nullptr)
	{
		m_root->Update(deltaTime);
	}
}

void Scene::End()
{
	if (m_root != nullptr)
	{
		m_root->End();
	}
}

void Scene::InitStatePlay(StatePlay* stateplay)
{
	if (stateplay != nullptr)
	{
        m_statePlay = stateplay;
	}
}

Actor* Scene::GetActor(Actor* actor, unsigned int entityID)
{
    if (m_root != nullptr && actor != nullptr)
    {
        if (actor->m_idEntity == entityID)
        {
            return actor;
        }

        if (actor->m_children.empty() == false)
        {
            auto children = actor->m_children;


            for (Actor* childActor : children)
            {
                if (childActor->m_idEntity == entityID)
                {
                    return childActor;
                }

            	if (childActor->m_children.empty() == false)
            	{
                    auto* ActorFound = GetActor(childActor, entityID);

            		if (ActorFound != nullptr)
            		{
                        return ActorFound;
            		}
            	}
            }
        }
       
    }

    return nullptr;
}

Actor* Scene::GetActorName(Actor* root, std::string name)
{
    if (m_root != nullptr && root != nullptr)
    {
        if (root->m_name == name)
        {
            return root;
        }

        if (root->m_children.empty() == false)
        {
            auto children = root->m_children;


            for (Actor* childActor : children)
            {
                if (childActor->m_name == name)
                {
                    return childActor;
                }

                if (childActor->m_children.empty() == false)
                {
                    auto* ActorFound = GetActorName(childActor, name);

                    if (ActorFound != nullptr)
                    {
                        return ActorFound;
                    }
                }
            }
        }

    }

    return nullptr;
}
