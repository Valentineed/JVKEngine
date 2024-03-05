#include "UIWindowWorldHierarchy.h"

#include "../../../../EngineEditor/sources/Scene/Actor.h"
#include "../../../../EngineEditor/sources/Scene/SceneManager.h"

UIWindowWorldHierarchy::UIWindowWorldHierarchy()
{
	m_windowName = "WorldHierarchyWindow";
}

void libgfx_API UIWindowWorldHierarchy::Update()
{
	DoOnceForThisWindow();

	if (m_bIsWindowMovable == true)
	{
		m_windowFlags = ImGuiWindowFlags_None;
	}
	else
	{
		m_windowFlags = ImGuiWindowFlags_NoMove;
	}

	//TODO: Change hard coding values later
	ImGui::SetNextWindowSizeConstraints(ImVec2(300.f, 815.f), ImVec2(300.f, 815.f));

	if (!Begin("WorldHierarchyWindow", m_windowFlags))
	{
		End();
	}
	else
	{

        SetWindowSpacePos();

        if ((ImGui::GetMousePos().x >= m_posTopLeftCorner.x && ImGui::GetMousePos().x <= m_posBottomRightCorner.x) &&
            (ImGui::GetMousePos().y >= m_posTopLeftCorner.y && ImGui::GetMousePos().y <= m_posBottomRightCorner.y))
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsAnyItemHovered() == false)
            {
                ImGui::OpenPopup("WorldHierarchyMainPopup");
            }
        }


        if (ImGui::BeginPopup("WorldHierarchyMainPopup"))
        {
            ImGui::Separator();

            if (ImGui::Button("Add new Actor"))
            {
                ImGui::OpenPopup("RenameAsset");

            }

            if (ImGui::BeginPopup("RenameAsset"))
            {
                if (ImGui::InputText("New name", m_actorName, 2048, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    m_createActorToRootDelegate(m_actorName);
                    ImGui::CloseCurrentPopup();
                    m_bShouldCloseMainPopup = true;
                    m_actorName[0] = '\0';
                }
                ImGui::EndPopup();
            }
            if (m_bShouldCloseMainPopup == true)
            {
                m_bShouldCloseMainPopup = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

		
        if (m_scene != nullptr)
        {
            DisplayRoot(m_scene->m_root);
        }

        
       

		End();
	}
}

void UIWindowWorldHierarchy::DoOnceForThisWindow()
{
	if (m_bDoOnce == true)
	{
		m_bDoOnce = false;
	}
}

void libgfx_API UIWindowWorldHierarchy::InitScene(Scene* scene)
{
	m_scene = scene;
}

void UIWindowWorldHierarchy::DisplayRoot(Actor* actor)
{
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static bool align_label_with_current_x_position = false;

    if (align_label_with_current_x_position)
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

    // 'selection_mask' is dumb representation of what may be user-side selection state.
    //  You may retain selection state inside or outside your objects in whatever format you see fit.
    // 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
    /// of the loop. May be a pointer to your own node type, etc.
    static int selection_mask = (1 << 2);
    int node_clicked = -1;

    ImGuiTreeNodeFlags node_flags = base_flags;
    const bool is_selected = (selection_mask & (1 << actor->m_idEntity)) != 0;
    if (is_selected)
    {
        node_flags |= ImGuiTreeNodeFlags_Selected;
    }
	
    if (actor != nullptr)
    {
        ImGui::PushID(actor->m_idEntity);
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)actor->m_idEntity, node_flags, actor->m_name.c_str(), actor->m_idEntity);
        if (ImGui::IsItemClicked())
        {
            node_clicked = actor->m_idEntity;
            m_node_clicked = node_clicked;
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("WorldHierarchyDragAndDrop"))
            {
                IM_ASSERT(payload->DataSize == sizeof(Actor));
                int payload_n = *(const int*)payload->Data;

                m_futureChildActorID = payload_n;
                m_futureParentActorID = actor->m_idEntity;
                m_bReparentActors = true;
            }
            ImGui::EndDragDropTarget();
        }

        if (node_open)
        {
            DisplayChildren(actor);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }


    if (node_clicked != -1)
    {
        // Update selection state
        // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
        if (ImGui::GetIO().KeyCtrl)
            selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
        else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
            selection_mask = (1 << node_clicked);           // Click to single-select
    }
}

void UIWindowWorldHierarchy::DisplayChildren(Actor* actor)
{

	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	static bool align_label_with_current_x_position = false;

	if (align_label_with_current_x_position)
		ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

	// 'selection_mask' is dumb representation of what may be user-side selection state.
	//  You may retain selection state inside or outside your objects in whatever format you see fit.
	// 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
	/// of the loop. May be a pointer to your own node type, etc.
	static int selection_mask = (1 << 2);
	int node_clicked = -1;

    if (actor->m_children.empty() == false)
    {
        for (Actor* dactor : actor->m_children)
        {
            ImGuiTreeNodeFlags node_flags = base_flags;
            const bool is_selected = (selection_mask & (1 << dactor->m_idEntity)) != 0;
            if (is_selected)
            {
                node_flags |= ImGuiTreeNodeFlags_Selected;
            }
            if (dactor->m_children.empty() == false)
            {
                ImGui::PushID(dactor->m_idEntity);
                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)dactor->m_idEntity, node_flags, dactor->m_name.c_str(), dactor->m_idEntity);
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    node_clicked = dactor->m_idEntity;
                    m_node_clicked = node_clicked;
                }
                else if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
                {
                    node_clicked = dactor->m_idEntity;
                    m_node_clicked = node_clicked;
                    ImGui::OpenPopup("RightClickActorPopup");
                }


                if (ImGui::BeginPopup("RightClickActorPopup"))
                {
                    ImGui::Separator();

                    if (ImGui::Button("Add new Actor"))
                    {
                        ImGui::OpenPopup("RenameAsset2");

                    }

                    if (ImGui::BeginPopup("RenameAsset2"))
                    {
                        if (ImGui::InputText("New name", m_actorName, 2048, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            m_createActorToActorDelegate(dactor, m_actorName);
                            ImGui::CloseCurrentPopup();
                            m_bShouldCloseMainPopup = true;
                            m_actorName[0] = '\0';
                        }
                        ImGui::EndPopup();
                    }
                    if (m_bShouldCloseMainPopup == true)
                    {
                        m_bShouldCloseMainPopup = false;
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }
                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("WorldHierarchyDragAndDrop", &dactor->m_idEntity, sizeof(Actor));
                    ImGui::Text("This is a drag and drop source");
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("WorldHierarchyDragAndDrop"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(Actor));
                        int payload_n = *(const int*)payload->Data;

                        m_futureChildActorID = payload_n;
                        m_futureParentActorID = dactor->m_idEntity;
                        m_bReparentActors = true;
                    }
                    ImGui::EndDragDropTarget();
                }

                if (node_open)
                {
                    DisplayChildren(dactor);
                    ImGui::TreePop();
                }
                ImGui::PopID();
            }
            else
            {
                ImGui::PushID(dactor->m_idEntity);
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                ImGui::TreeNodeEx((void*)(intptr_t)dactor->m_idEntity, node_flags, dactor->m_name.c_str(), dactor->m_idEntity);

                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    node_clicked = dactor->m_idEntity;
                    m_node_clicked = node_clicked;
                }
                else if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
                {
                    node_clicked = dactor->m_idEntity;
                    m_node_clicked = node_clicked;
                    ImGui::OpenPopup("RightClickActorPopup");
                }


                if (ImGui::BeginPopup("RightClickActorPopup"))
                {
                    ImGui::Separator();

                    if (ImGui::Button("Add new Actor"))
                    {
                        ImGui::OpenPopup("RenameAsset2");

                    }

                     if (ImGui::BeginPopup("RenameAsset2"))
                     {
                         if (ImGui::InputText("New name", m_actorName, 2048, ImGuiInputTextFlags_EnterReturnsTrue))
                         {
                             m_createActorToActorDelegate(dactor, m_actorName);
                             ImGui::CloseCurrentPopup();
                             m_bShouldCloseMainPopup = true;
                             m_actorName[0] = '\0';
                         }
                         ImGui::EndPopup();
                     }
                     if (m_bShouldCloseMainPopup == true)
                     {
                         m_bShouldCloseMainPopup = false;
                         ImGui::CloseCurrentPopup();
                     }

                    ImGui::EndPopup();
                }

            	
                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("WorldHierarchyDragAndDrop", &dactor->m_idEntity, sizeof(Actor));
                    ImGui::Text("This is a drag and drop source");
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("WorldHierarchyDragAndDrop"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(Actor));
                        int payload_n = *(const int*)payload->Data;

                        m_futureChildActorID = payload_n;
                        m_futureParentActorID = dactor->m_idEntity;
                        m_bReparentActors = true;
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::PopID();
            }

        }
    }

	if (node_clicked != -1)
	{
		// Update selection state
		// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
		if (ImGui::GetIO().KeyCtrl)
			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
			selection_mask = (1 << node_clicked);           // Click to single-select
	}
}

int libgfx_API UIWindowWorldHierarchy::GetActorSelectedID()
{
    return m_node_clicked;
}

void libgfx_API UIWindowWorldHierarchy::SetActorSelectedID(int ID)
{
	m_node_clicked = ID;
}

Actor* UIWindowWorldHierarchy::GetDragSourceActor(Actor* actor, unsigned int entityID)
{
    if (m_scene->m_root != nullptr && actor != nullptr)
    {
        if (actor->m_idEntity == entityID)
        {
            return actor;
        }

        auto children = actor->m_children;

        for (Actor* childActor : children)
        {
            if (childActor->m_idEntity == entityID)
            {
                return childActor;
            }
        	
            GetDragSourceActor(childActor, entityID);
        }
    }

    return nullptr;
}

unsigned int libgfx_API UIWindowWorldHierarchy::GetFutureParentActorID()
{
    return m_futureParentActorID;
}

unsigned int libgfx_API UIWindowWorldHierarchy::GetFutureChildActorID()
{
    return m_futureChildActorID;
}

bool libgfx_API UIWindowWorldHierarchy::IsReparentingActors()
{
    return m_bReparentActors;
}

void UIWindowWorldHierarchy::SetReparentingActors(bool tf)
{
    m_bReparentActors = tf;
}

Delegate<void(std::string)>& UIWindowWorldHierarchy::GetCreateActorToRootDelegate()
{
    return m_createActorToRootDelegate;
}

libgfx_API Delegate<void(Actor*, std::string)>& UIWindowWorldHierarchy::GetCreateActorToActorDelegate()
{
    return m_createActorToActorDelegate;
}
