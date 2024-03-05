#pragma once
#include <stack>
#include <vector>

#include "../Components/Transform.h"
#include "../ECS/System.h"

class Scene;

class TransformSystem : public System
{
public:
	void Update();
	[[nodiscard]] size_t Register();
	Transform* GetTransform(size_t index);
	//void ChangeTransformAt(size_t index);
	void RemoveChild(size_t indexParent, size_t indexChild, bool lastChild);
	void AddChild(size_t indexParent, size_t indexChild);
	void CleanUpSystem();
	//Todo use ObjectPool to reuse inutil transform (Actors delete)
	std::vector<Transform> m_transforms;

	//keep all the indexes in which the tranform has been modified
	std::vector<size_t> modifyTindex;
	bool bDataChange = false;
protected:
	static  const  size_t s_invalidIndex = SIZE_MAX;
private:
	std::stack<size_t> m_freeIndex;
	//Update all children recursively
	void UpdateChildren(std::vector<size_t> children, size_t curIndex);
	
	/*this function set modifyTindex
	keep only indexes or all these parents have not been modified*/
	void SearchAllTransfsormModify();
};
