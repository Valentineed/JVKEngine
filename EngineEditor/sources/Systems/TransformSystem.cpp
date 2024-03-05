#include "TransformSystem.h"

#include "PhysixSystem.h"
#include "../ECS/World.h"
#include "../Components/Transform.h"
#include "../Scene/Actor.h"
#include "../Scene/SceneManager.h"
//TODO recalculate m_localtransform if parent change

void TransformSystem::Update()
{
	bDataChange = false;
	if(!modifyTindex.empty())
	{
		bDataChange = true;
		SearchAllTransfsormModify();
		
		for (size_t index : modifyTindex)
		{
			if (m_transforms[index].indexParent != s_invalidIndex)
			{
				m_transforms[index].m_worldTransform = m_transforms[m_transforms[index].indexParent].m_worldTransform * m_transforms[index].m_localTransform;
				m_transforms[index].UpdateVectors();
			}
			else
			{
				m_transforms[index].m_worldTransform = m_transforms[index].m_localTransform;
			}
			UpdateChildren(m_transforms[index].indexChildren, index);
			m_transforms[index].bModify = false;
		}
		modifyTindex.clear();
	}
	
}

size_t TransformSystem::Register()
{
	if (m_freeIndex.empty())
	{
		size_t index = m_transforms.size();
		auto& transform = m_transforms.emplace_back();
		transform.myIndexSystem = index;
		return index;
	}
	const size_t index = m_freeIndex.top();
	m_freeIndex.pop();
	m_transforms[index] = {};
	m_transforms[index].myIndexSystem = index;
	return index;
}

Transform* TransformSystem::GetTransform(size_t index)
{
	return &m_transforms[index];
}

void TransformSystem::RemoveChild(size_t indexParent, size_t indexChild, bool /*lastChild*/)
{
	if (!m_transforms[indexChild].indexChildren.empty())
	{
		for (auto indexSystemChild : m_transforms[indexChild].indexChildren)
		{
			m_transforms[indexSystemChild].indexParent = indexParent;
		}
	}
	if(m_transforms[indexParent].indexChildren.size()-1 != m_transforms[indexChild].indexInParentVec)
	{
		m_transforms[m_transforms[indexParent].indexChildren.back()].indexInParentVec = m_transforms[indexChild].indexInParentVec;
		std::iter_swap(m_transforms[indexParent].indexChildren.begin() + m_transforms[indexChild].indexInParentVec, m_transforms[indexParent].indexChildren.end() - 1);
	}
	m_transforms[m_transforms[indexChild].indexParent].indexChildren.pop_back();
	m_freeIndex.push(m_transforms[indexChild].myIndexSystem);
}

void TransformSystem::AddChild(size_t indexParent, size_t indexChild)
{
	const size_t index = m_transforms[indexParent].indexChildren.size();
	m_transforms[indexParent].indexChildren.push_back(indexChild);
	
	if(m_transforms[indexChild].indexParent != s_invalidIndex)
	{
		size_t indexOldParent = m_transforms[indexChild].indexParent;
		
		auto children = std::find(m_transforms[indexOldParent].indexChildren.begin(),
			m_transforms[indexOldParent].indexChildren.end(), indexChild);

		assert(children != m_transforms[indexOldParent].indexChildren.end());

		std::iter_swap(children,
			m_transforms[indexOldParent].indexChildren.end() - 1);
		m_transforms[indexOldParent].indexChildren.pop_back();

	}

	m_transforms[indexChild].indexInParentVec = index;
	m_transforms[indexChild].indexParent = indexParent;
	if(std::find(modifyTindex.begin(), modifyTindex.end(), indexChild) == modifyTindex.end())
	{
		m_transforms[indexChild].indexParent = indexParent;
		m_transforms[indexChild].indexInParentVec = index;
	}
	else
	{
		modifyTindex.push_back(indexChild);
	}

}

void TransformSystem::CleanUpSystem()
{
	modifyTindex.clear();
}

void TransformSystem::UpdateChildren(std::vector<size_t> children, size_t curIndex)
{
	for (size_t childIndex : children)
	{
		m_transforms[childIndex].m_worldTransform = m_transforms[curIndex].m_worldTransform * m_transforms[childIndex].m_localTransform;
		m_transforms[childIndex].UpdateVectors();
		m_transforms[childIndex].bModify = false;
		if(!m_transforms[childIndex].indexChildren.empty())
		{
			UpdateChildren(m_transforms[childIndex].indexChildren, childIndex);
		}
	}
}

void TransformSystem::SearchAllTransfsormModify()
{
	//https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
	modifyTindex.erase(std::remove_if(modifyTindex.begin(), modifyTindex.end(), [this](size_t modifyindex)
		{
			size_t currentTransform = modifyindex;
			while (currentTransform != s_invalidIndex)
			{
				
				if (m_transforms[currentTransform].indexParent != s_invalidIndex
					&& m_transforms[m_transforms[currentTransform].indexParent].bModify)
				{
					return true;
				}
				currentTransform = m_transforms[currentTransform].indexParent;

			}
			return false;
		}
	), modifyTindex.end());
}
