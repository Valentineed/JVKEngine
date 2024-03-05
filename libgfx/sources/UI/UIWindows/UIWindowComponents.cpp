#include "UIWindowComponents.h"
#include "Resources/TextureLoader.h"


#include <windows.h>

#include "UI/UIWidgets.h"

#include "ToolsEngine.h"

UIWindowComponents::UIWindowComponents()
{
	m_windowName = "ComponentsWindow";

	GetPhysicComponentData().GetRigidTypes().emplace_back("Dynamic");
	GetPhysicComponentData().GetRigidTypes().emplace_back("Static");
}

void libgfx_API UIWindowComponents::Update()
{
	UIWindow::Update();

	DoOnceForThisWindow();

	if (m_bIsWindowMovable == true)
	{
		m_windowFlags = ImGuiWindowFlags_None;
	}
	else
	{
		m_windowFlags = ImGuiWindowFlags_NoMove;
	}

	if (!Begin("ComponentsWindow", m_windowFlags))
	{
		End();
	}
	else
	{

		SetWindowSpacePos();

		
		if (m_ActorIDToDisplay != -1)
		{
			ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerH;

			if (ImGui::BeginTable("##ComponentsTable", 2, flags))
			{
				ImGui::TableSetupColumn("TableSetup", ImGuiTableColumnFlags_WidthFixed);
				
				DisplayBasicActorSettings();
				if (m_bIsTransformComponentEligible == true)
				{
					ImGui::TableNextRow();
					DisplayTransformComponent();
				}
				if (m_bIsMeshComponentEligible == true)
				{
					ImGui::TableNextRow();
					DisplayMeshComponent();
				}
				if (m_bIsPhysixComponentEligible == true)
				{
					ImGui::TableNextRow();
					DisplayPhysixComponent();
				}
				if (m_bIsCameraComponentEligible == true)
				{
					ImGui::TableNextRow();
					DisplayCameraComponent();
				}
				if (m_bIsLightComponentEligible == true)
				{
					ImGui::TableNextRow();
					DisplayLightComponent();
				}

				if (m_bIsScriptComponentEligible == true)
				{
					ImGui::TableNextRow();
					DisplayScriptsComponent();
				}

				if (m_bIsSoundComponentEligible == true)
				{
					ImGui::TableNextRow();
					DisplaySoundComponent();
				}
				ImGui::EndTable();
			}
		}
		End();
	}
}

int libgfx_API UIWindowComponents::GetActorIDToDisplay()
{
	return m_ActorIDToDisplay;
}

void libgfx_API UIWindowComponents::SetActorIDToDisplay(int ID)
{
	m_ActorIDToDisplay = ID;
}

void libgfx_API UIWindowComponents::FindAndSetActorToDisplay(Actor* actor, unsigned int ActorIDToFind)
{
	if (ActorIDToFind != -1)
	{
		if (actor->m_children.empty() == false)
		{
			for (Actor* dactor : actor->m_children)
			{
				if (dactor->m_idEntity == ActorIDToFind)
				{
					m_actorToDisplay = dactor;
					return;
				}
				FindAndSetActorToDisplay(dactor, ActorIDToFind);
			}
		}
	}
}

libgfx_API Delegate<void(unsigned int, const std::string&)>& UIWindowComponents::GetScriptCreationDelegate()
{
	return m_createScriptComponentDelegate;
}

libgfx_API Delegate<void(unsigned int, Vector3, Vector3, Vector3)>& UIWindowComponents::GetCameraCreationDelegate()
{
	return m_createCameraComponentDelegate;
}

libgfx_API Delegate<void(unsigned int, size_t)>& UIWindowComponents::GetMeshCreationDelegate()
{
	return m_createMeshComponentDelegate;
}

libgfx_API Delegate<void(unsigned int)>& UIWindowComponents::GetSphereShapePhysicsCreationDelegate()
{
	return m_createSphereShapePhysicsComponentDelegate;
}

libgfx_API Delegate<void(unsigned int)>& UIWindowComponents::GetCapsuleShapePhysicsCreationDelegate()
{
	return m_createCapsuleShapePhysicsComponentDelegate;
}

libgfx_API Delegate<void(unsigned int)>& UIWindowComponents::GetBoxShapePhysicsCreationDelegate()
{
	return m_createBoxShapePhysicsComponentDelegate;
}

libgfx_API Delegate<void(unsigned int)>& UIWindowComponents::GetDirectionalLightCreationDelegate()
{
	return m_createDirectionalLightComponentDelegate;
}

libgfx_API Delegate<void(unsigned int, Vector3)>& UIWindowComponents::GetSpotLightCreationDelegate()
{
	return m_createSpotLightComponentDelegate;
}

libgfx_API Delegate<void(unsigned int)>& UIWindowComponents::GetPointLightCreationDelegate()
{
	return m_createPointLightComponentDelegate;
}

libgfx_API Delegate<void(unsigned int)>& UIWindowComponents::GetSoundCreationDelegate()
{
	return m_createSoundComponentDelegate;
}

libgfx_API Delegate<void(Actor*)>& UIWindowComponents::GetRemoveActorDelegate()
{
	return m_removeActorDelegate;
}

Actor* UIWindowComponents::GetActorSelected()
{
	return m_actorToDisplay;
}

void UIWindowComponents::DisplayActorTransform()
{
	IUIWidgets::CreateComponentContentTitle("Position");
	
	IUIWidgets::CreateVec3Content(m_textures[static_cast<int>(TextureNameComponentsWindow::X_AXIS)].GetTextureID(),
		m_textures[static_cast<int>(TextureNameComponentsWindow::Y_AXIS)].GetTextureID(),
		m_textures[static_cast<int>(TextureNameComponentsWindow::Z_AXIS)].GetTextureID(),
		"##PosX", "##PosY", "##PosZ", GetTransformComponentData().GetPosition(), m_bIsTransformUpdated);

	IUIWidgets::CreateComponentContentTitle("Rotation");
	
	IUIWidgets::CreateVec3Content(m_textures[static_cast<int>(TextureNameComponentsWindow::X_AXIS)].GetTextureID(),
		m_textures[static_cast<int>(TextureNameComponentsWindow::Y_AXIS)].GetTextureID(),
		m_textures[static_cast<int>(TextureNameComponentsWindow::Z_AXIS)].GetTextureID(),
		"##RotX", "##RotY", "##RotZ", GetTransformComponentData().GetRotation(), m_bIsTransformUpdated);

	IUIWidgets::CreateComponentContentTitle("Scale");
	
	IUIWidgets::CreateVec3Content(m_textures[static_cast<int>(TextureNameComponentsWindow::X_AXIS)].GetTextureID(),
		m_textures[static_cast<int>(TextureNameComponentsWindow::Y_AXIS)].GetTextureID(),
		m_textures[static_cast<int>(TextureNameComponentsWindow::Z_AXIS)].GetTextureID(),
		"##ScaX", "##ScaY", "##ScaZ", GetTransformComponentData().GetScale(), m_bIsTransformUpdated);
}

void UIWindowComponents::DisplayBasicActorSettings()
{
	IUIWidgets::CreateComponentContentTitle("Actor Name");
	if (ImGui::InputText("##ActorNameInput", m_actorToDisplay->m_actorNameInput, 2048, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		m_actorToDisplay->m_name = m_actorToDisplay->m_actorNameInput;
	}
	IUIWidgets::CreateComponentContentTitle("Actor ID");
	IUIWidgets::CreateText(std::to_string(m_actorToDisplay->m_idEntity));
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	if(ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("Popup Add Component");
	}

	ImGui::TableSetColumnIndex(1);
	if (ImGui::Button("Remove actor"))
	{
		//TODO Fix delegate
		m_bIsActorRemoved = true;
		//m_removeActorDelegate(m_actorToDisplay);
	}

	if (ImGui::BeginPopup("Popup Add Component"))
	{
		if (ImGui::BeginCombo("Components available", m_ComponentNameToAddSelected.c_str()))
		{
			if (m_bIsMeshComponentEligible == false)
			{
				if (ImGui::Selectable("Mesh"))
				{
					m_ComponentNameToAddSelected = "Mesh";
				}
			}
			if (m_bIsPhysixComponentEligible == false)
			{
				if (ImGui::Selectable("Sphere shape physics"))
				{
					m_ComponentNameToAddSelected = "SphereShapePhysics";
				}

				if (ImGui::Selectable("Capsule shape physics"))
				{
					m_ComponentNameToAddSelected = "CapsuleShapePhysics";
				}

				if (ImGui::Selectable("Box shape physics"))
				{
					m_ComponentNameToAddSelected = "BoxShapePhysics";
				}
			}

			if (m_bIsCameraComponentEligible == false)
			{
				if (ImGui::Selectable("Camera"))
				{
					m_ComponentNameToAddSelected = "Camera";
				}
			}

			if (m_bIsLightComponentEligible == false)
			{
				if (ImGui::Selectable("Directional Light"))
				{
					m_ComponentNameToAddSelected = "Directional Light";
				}

				if (ImGui::Selectable("Spot Light"))
				{
					m_ComponentNameToAddSelected = "Spot Light";
				}

				if (ImGui::Selectable("Point Light"))
				{
					m_ComponentNameToAddSelected = "Point Light";
				}
			}

			if (m_bIsScriptComponentEligible == false)
			{
				if (ImGui::Selectable("Script"))
				{
					m_ComponentNameToAddSelected = "Script";
				}
			}

			if (m_bIsSoundComponentEligible == false)
			{
				if (ImGui::Selectable("Sound"))
				{
					m_ComponentNameToAddSelected = "Sound";
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Add new component"))
		{
			if (m_ComponentNameToAddSelected == "Camera")
			{
				m_createCameraComponentDelegate(m_actorToDisplay->m_idEntity, GetTransformComponentData().GetPosition(), { 0.f, 0.f, -1.f }, { 0.f, 1.f, 0.f });
				m_bReloadActorComponentsData = true;
			}

			if (m_ComponentNameToAddSelected == "Mesh")
			{
				m_createMeshComponentDelegate(m_actorToDisplay->m_idEntity, m_actorToDisplay->m_indexTransform);
				m_bReloadActorComponentsData = true;
			}

			if (m_ComponentNameToAddSelected == "SphereShapePhysics")
			{
				m_createSphereShapePhysicsComponentDelegate(m_actorToDisplay->m_idEntity);
				m_bReloadActorComponentsData = true;
			}

			if (m_ComponentNameToAddSelected == "CapsuleShapePhysics")
			{
				m_createCapsuleShapePhysicsComponentDelegate(m_actorToDisplay->m_idEntity);
				m_bReloadActorComponentsData = true;
			}

			if (m_ComponentNameToAddSelected == "BoxShapePhysics")
			{
				m_createBoxShapePhysicsComponentDelegate(m_actorToDisplay->m_idEntity);
				m_bReloadActorComponentsData = true;
			}

			if (m_ComponentNameToAddSelected == "Directional Light")
			{
				m_createDirectionalLightComponentDelegate(m_actorToDisplay->m_idEntity);
				m_bReloadActorComponentsData = true;
			}

			if (m_ComponentNameToAddSelected == "Spot Light")
			{
				m_createSpotLightComponentDelegate(m_actorToDisplay->m_idEntity, GetTransformComponentData().GetPosition());
				m_bReloadActorComponentsData = true;
			}

			if (m_ComponentNameToAddSelected == "Point Light")
			{
				m_createPointLightComponentDelegate(m_actorToDisplay->m_idEntity);
				m_bReloadActorComponentsData = true;
			}

			if (m_ComponentNameToAddSelected == "Script")
			{
				std::string PathFound = ToolsEngine::OpenFileName();

				if (PathFound.empty() == false)
				{
					if (PathFound.find(".lua") != std::string::npos) {
						m_createScriptComponentDelegate(m_actorToDisplay->m_idEntity, PathFound);
						m_bReloadActorComponentsData = true;
					}
					else
					{
						MessageBox(NULL, "The path given was not a lua file", "Error adding script", MB_OK);
					}
				}
			}

			if (m_ComponentNameToAddSelected == "Sound")
			{
				m_createSoundComponentDelegate(m_actorToDisplay->m_idEntity);
				m_bReloadActorComponentsData = true;
			}

			m_ComponentNameToAddSelected.clear();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void UIWindowComponents::DisplayTransformComponent()
{
	if (IUIWidgets::CreateHeaderComponent("Transform"))
	{
		DisplayActorTransform();
		ImGui::TreePop();
	}
}

void UIWindowComponents::DisplayMeshComponent()
{
	if (IUIWidgets::CreateHeaderComponent("Static Mesh"))
	{
		IUIWidgets::CreateComponentContentTitle("Static Mesh");
		std::string SelectableImageName = "OBJSelectImage";
		auto OldImagePos = ImGui::GetCursorPos();
		if (ImGui::Selectable(SelectableImageName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100)))
		{
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				std::string PathFound = ToolsEngine::OpenFileName();

				if (PathFound.empty() == false)
				{
					GetStaticMeshComponentData().ChangeStaticMesh(PathFound);
				}
			}
		}
		ImGui::SetCursorPos(OldImagePos);
		ImGui::Image(m_textures[static_cast<int>(TextureNameComponentsWindow::BLACK_BACKGROUND)].GetTextureID(), ImVec2(100, 100));

		ImGui::SameLine();

		IUIWidgets::CreateText(GetStaticMeshComponentData().GetStaticMeshName());

		/////////////TEXTURE
		IUIWidgets::CreateComponentContentTitle("Mesh Texture");
		OldImagePos = ImGui::GetCursorPos();
		if (ImGui::Selectable(GetStaticMeshComponentData().GetNewMeshTexturePath().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100)))
		{
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				std::string PathFound = ToolsEngine::OpenFileName();

				if (PathFound.empty() == false)
				{
					GetStaticMeshComponentData().ChangeStaticMeshTexture(PathFound);
				}
			}
		}
		ImGui::SetCursorPos(OldImagePos);
		if (GetStaticMeshComponentData().GetStaticMeshTextureName().empty() == false)
		{
			ImGui::Image(m_textures[static_cast<int>(TextureNameComponentsWindow::CURRENT_MESH_TEXTURE)].GetTextureID(), ImVec2(100, 100));
		}
		else
		{
			ImGui::Image(m_textures[static_cast<int>(TextureNameComponentsWindow::BLACK_BACKGROUND)].GetTextureID(), ImVec2(100, 100));
		}

		ImGui::SameLine();
		IUIWidgets::CreateText(GetStaticMeshComponentData().GetStaticMeshTextureName());

		ImGui::TreePop();
	}
	IUIWidgets::OnRightClickComponent("Remove Static Mesh Component", m_componentNameToRemove);
}

void UIWindowComponents::DisplayPhysixComponent()
{
	if (IUIWidgets::CreateHeaderComponent("Physics"))
	{
		IUIWidgets::CreateComponentContentTitle("Shape type");
		ImGui::Text(GetPhysicComponentData().GetShape().c_str());
		
		IUIWidgets::CreateComponentContentTitle("Physics");

		if (GetPhysicComponentData().GetCurrentRigidType() == 0)
		{
			if (ImGui::Checkbox("Is Kinematic", &GetPhysicComponentData().IsKinematic()))
			{
				m_bIsPhysicsUpdated = true;
			}

			if (ImGui::Checkbox("Disable Gravity", &GetPhysicComponentData().IsGravityDisabled()))
			{
				m_bIsPhysicsUpdated = true;
			}

			if (ImGui::InputFloat("Mass", &GetPhysicComponentData().GetMass()))
			{
				m_bIsPhysicsUpdated = true;
			}
		}
	
		
		if(ImGui::Checkbox("Trigger", &GetPhysicComponentData().IsTrigger()))
		{
			m_bIsPhysicsUpdated = true;
		}


		if (ImGui::BeginCombo("Physic Type", GetPhysicComponentData().GetRigidTypes()[GetPhysicComponentData().GetCurrentRigidType()].c_str()))
		{
			for (int n = 0; n < GetPhysicComponentData().GetRigidTypes().size(); n++)
			{
				const bool is_selected = (GetPhysicComponentData().GetCurrentRigidType() == n);
				if (ImGui::Selectable(GetPhysicComponentData().GetRigidTypes()[n].c_str(), is_selected))
				{
					GetPhysicComponentData().SetCurrentRigidType(n);
					m_bIsRigidTypeUpdated = true;
				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		
		IUIWidgets::CreateComponentContentTitle("Lock Rotation");
		
		if(ImGui::Checkbox("X", &GetPhysicComponentData().IsRotationXLocked()))
		{
			m_bIsPhysicsUpdated = true;
		}
		ImGui::SameLine();
		if(ImGui::Checkbox("Y", &GetPhysicComponentData().IsRotationYLocked()))
		{
			m_bIsPhysicsUpdated = true;
		}
		ImGui::SameLine();
		if(ImGui::Checkbox("Z", &GetPhysicComponentData().IsRotationZLocked()))
		{
			m_bIsPhysicsUpdated = true;
		}

		if (GetPhysicComponentData().GetShape() != "Convex")
		{
			if (GetPhysicComponentData().GetShape() == "Box")
			{
				IUIWidgets::CreateComponentContentTitle("Scale");
				
				IUIWidgets::CreateVec3Content(m_textures[static_cast<int>(TextureNameComponentsWindow::X_AXIS)].GetTextureID(),
					m_textures[static_cast<int>(TextureNameComponentsWindow::Y_AXIS)].GetTextureID(),
					m_textures[static_cast<int>(TextureNameComponentsWindow::Z_AXIS)].GetTextureID(),
					"##ScaPX", "##ScaPY", "##ScaPZ", GetPhysicComponentData().GetScalePhysic(), m_bIsPhysicsUpdated);
			}
			else if (GetPhysicComponentData().GetShape() == "Capsule")
			{
				IUIWidgets::CreateComponentContentTitle("Radius");
				if (ImGui::InputFloat("Radius##ScalePhysic", &GetPhysicComponentData().GetScalePhysic().x, 0, 0, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					m_bIsPhysicsUpdated = true;
				}
				IUIWidgets::CreateComponentContentTitle("Half Height");
				if (ImGui::InputFloat("Half Height##ScalePhysic", &GetPhysicComponentData().GetScalePhysic().y, 0, 0, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					m_bIsPhysicsUpdated = true;
				}
			}
			else if (GetPhysicComponentData().GetShape() == "Sphere")
			{
				IUIWidgets::CreateComponentContentTitle("Radius");
				if (ImGui::InputFloat("Radius##ScalePhysic", &GetPhysicComponentData().GetScalePhysic().x,0, 0, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				{
					m_bIsPhysicsUpdated = true;
				}
			}
		}
		
		
		
		

		ImGui::TreePop();
	}
	IUIWidgets::OnRightClickComponent("Remove Physix Component", m_componentNameToRemove);
}

void UIWindowComponents::DisplayCameraComponent()
{
	if (IUIWidgets::CreateHeaderComponent("Camera"))
	{
		IUIWidgets::CreateComponentContentTitle("Camera offset", true,"Camera offset from the actor position" );

		IUIWidgets::CreateVec3Content(m_textures[static_cast<int>(TextureNameComponentsWindow::X_AXIS)].GetTextureID(),
			m_textures[static_cast<int>(TextureNameComponentsWindow::Y_AXIS)].GetTextureID(),
			m_textures[static_cast<int>(TextureNameComponentsWindow::Z_AXIS)].GetTextureID(),
			"##CameraOffsetPosX", "##CameraOffsetPosY", "##CameraOffsetPosZ", GetCameraComponentData().GetCameraOffset(), m_bIsCameraUpdated);
		
		ImGui::TreePop();
	}
	IUIWidgets::OnRightClickComponent("Remove Camera Component", m_componentNameToRemove);
}

void UIWindowComponents::DisplayLightComponent()
{
	if (IUIWidgets::CreateHeaderComponent("Light"))
	{
		IUIWidgets::CreateComponentContentTitle("Light Type");
		IUIWidgets::CreateText(GetLightComponentData().GetLightType());
		
		IUIWidgets::CreateComponentContentTitle("Ambiant Color");
		if (GetLightComponentData().GetLightType() == "Directional")
		{
			IUIWidgets::CreateColorButtonAndPickerOnPopup("##AmbiantColor", "MyColor##4", "Popup AmbiantColor", GetLightComponentData().GetAmbiantColorLight(), m_bIsDirectionalLightUpdated);

			IUIWidgets::CreateComponentContentTitle("Diffuse Color");
			IUIWidgets::CreateColorButtonAndPickerOnPopup("##DiffuseColor", "MyColor##5", "Popup DiffuseColor", GetLightComponentData().GetDiffuseColorLight(), m_bIsDirectionalLightUpdated);

			IUIWidgets::CreateComponentContentTitle("Specular Color");
			IUIWidgets::CreateColorButtonAndPickerOnPopup("##SpecularColor", "MyColor##6", "Popup SpecularColor", GetLightComponentData().GetSpecularColorLight(), m_bIsDirectionalLightUpdated);

			IUIWidgets::CreateComponentContentTitle("Position");

			IUIWidgets::CreateVec3Content(m_textures[static_cast<int>(TextureNameComponentsWindow::X_AXIS)].GetTextureID(),
				m_textures[static_cast<int>(TextureNameComponentsWindow::Y_AXIS)].GetTextureID(),
				m_textures[static_cast<int>(TextureNameComponentsWindow::Z_AXIS)].GetTextureID(),
				"##PosXLight", "##PosYLight", "##PosZLight", GetLightComponentData().GetLightPosition(), m_bIsDirectionalLightUpdated);
		}
		else if (GetLightComponentData().GetLightType() == "Spotlight")
		{
			IUIWidgets::CreateColorButtonAndPickerOnPopup("##AmbiantColor", "MyColor##4", "Popup AmbiantColor", GetLightComponentData().GetAmbiantColorLight(), m_bIsSpotLightLightUpdated);

			IUIWidgets::CreateComponentContentTitle("Diffuse Color");
			IUIWidgets::CreateColorButtonAndPickerOnPopup("##DiffuseColor", "MyColor##5", "Popup DiffuseColor", GetLightComponentData().GetDiffuseColorLight(), m_bIsSpotLightLightUpdated);

			IUIWidgets::CreateComponentContentTitle("Specular Color");
			IUIWidgets::CreateColorButtonAndPickerOnPopup("##SpecularColor", "MyColor##6", "Popup SpecularColor", GetLightComponentData().GetSpecularColorLight(), m_bIsSpotLightLightUpdated);

			IUIWidgets::CreateComponentContentTitle("Position");

			IUIWidgets::CreateVec3Content(m_textures[static_cast<int>(TextureNameComponentsWindow::X_AXIS)].GetTextureID(),
				m_textures[static_cast<int>(TextureNameComponentsWindow::Y_AXIS)].GetTextureID(),
				m_textures[static_cast<int>(TextureNameComponentsWindow::Z_AXIS)].GetTextureID(),
				"##PosXLight", "##PosYLight", "##PosZLight", GetLightComponentData().GetLightPosition(), m_bIsSpotLightLightUpdated);
		}
		else if (GetLightComponentData().GetLightType() == "Pointlight")
		{
			IUIWidgets::CreateColorButtonAndPickerOnPopup("##AmbiantColor", "MyColor##4", "Popup AmbiantColor", GetLightComponentData().GetAmbiantColorLight(), m_bIsPointLightLightUpdated);

			IUIWidgets::CreateComponentContentTitle("Diffuse Color");
			IUIWidgets::CreateColorButtonAndPickerOnPopup("##DiffuseColor", "MyColor##5", "Popup DiffuseColor", GetLightComponentData().GetDiffuseColorLight(), m_bIsPointLightLightUpdated);

			IUIWidgets::CreateComponentContentTitle("Specular Color");
			IUIWidgets::CreateColorButtonAndPickerOnPopup("##SpecularColor", "MyColor##6", "Popup SpecularColor", GetLightComponentData().GetSpecularColorLight(), m_bIsPointLightLightUpdated);

			IUIWidgets::CreateComponentContentTitle("Position");

			IUIWidgets::CreateVec3Content(m_textures[static_cast<int>(TextureNameComponentsWindow::X_AXIS)].GetTextureID(),
				m_textures[static_cast<int>(TextureNameComponentsWindow::Y_AXIS)].GetTextureID(),
				m_textures[static_cast<int>(TextureNameComponentsWindow::Z_AXIS)].GetTextureID(),
				"##PosXLight", "##PosYLight", "##PosZLight", GetLightComponentData().GetLightPosition(), m_bIsPointLightLightUpdated);
		}

		


		if (GetLightComponentData().GetLightType() == "Spotlight")
		{
			IUIWidgets::CreateComponentContentTitle("Direction");
			IUIWidgets::CreateVec3Content(m_textures[static_cast<int>(TextureNameComponentsWindow::X_AXIS)].GetTextureID(),
				m_textures[static_cast<int>(TextureNameComponentsWindow::Y_AXIS)].GetTextureID(),
				m_textures[static_cast<int>(TextureNameComponentsWindow::Z_AXIS)].GetTextureID(),
				"##DirXLight", "##DirYLight", "##DirZLight", GetLightComponentData().GetLightDirection(), m_bIsSpotLightLightUpdated);

			IUIWidgets::CreateComponentContentTitle("CutOff");
			if(ImGui::InputFloat("##LightCutOff", &GetLightComponentData().GetLightCutOff()))
			{
				m_bIsSpotLightLightUpdated = true;
			}

			IUIWidgets::CreateComponentContentTitle("OuterCutOff");
			if (ImGui::InputFloat("##LightOuterCutOff", &GetLightComponentData().GetLightOuterCutOff()))
			{
				m_bIsSpotLightLightUpdated = true;
			}
		}

		if (GetLightComponentData().GetLightType() == "Spotlight" || GetLightComponentData().GetLightType() == "Pointlight")
		{
			IUIWidgets::CreateComponentContentTitle("Constant Attenuation");

			if (GetLightComponentData().GetLightType() == "Spotlight")
			{
				if (ImGui::InputFloat("##constAtt", &GetLightComponentData().GetConstantAttenuation()))
				{
					m_bIsSpotLightLightUpdated = true;
				}

				IUIWidgets::CreateComponentContentTitle("Linear Attenuation");
				if (ImGui::InputFloat("##linAtt", &GetLightComponentData().GetLinearAttenuation()))
				{
					m_bIsSpotLightLightUpdated = true;
				}

				IUIWidgets::CreateComponentContentTitle("Quadratic Attenuation");
				if (ImGui::InputFloat("##quadAtt", &GetLightComponentData().GetQuadraticAttenuation()))
				{
					m_bIsSpotLightLightUpdated = true;
				}
			}
			else if (GetLightComponentData().GetLightType() == "Pointlight")
			{
				if (ImGui::InputFloat("##constAtt", &GetLightComponentData().GetConstantAttenuation()))
				{
					m_bIsPointLightLightUpdated = true;
				}

				IUIWidgets::CreateComponentContentTitle("Linear Attenuation");
				if (ImGui::InputFloat("##linAtt", &GetLightComponentData().GetLinearAttenuation()))
				{
					m_bIsPointLightLightUpdated = true;
				}

				IUIWidgets::CreateComponentContentTitle("Quadratic Attenuation");
				if (ImGui::InputFloat("##quadAtt", &GetLightComponentData().GetQuadraticAttenuation()))
				{
					m_bIsPointLightLightUpdated = true;
				}
			}
			
		}					
		ImGui::TreePop();
	}
	IUIWidgets::OnRightClickComponent("Remove Light Component", m_componentNameToRemove);
}

void UIWindowComponents::DisplayScriptsComponent()
{
	if (IUIWidgets::CreateHeaderComponent("Scripts"))
	{
		IUIWidgets::CreateComponentContentTitle("Scripts");
		for (std::string fileName : GetScriptComponentData().GetScriptsPath())
		{
			ImGui::Image(m_textures[static_cast<int>(TextureNameComponentsWindow::BLACK_BACKGROUND)].GetTextureID(), ImVec2(50, 50));
			ImGui::SameLine();
			ImGui::Text(fileName.c_str());
		}
		ImGui::TreePop();
	}
}

void UIWindowComponents::DisplaySoundComponent()
{
	if (IUIWidgets::CreateHeaderComponent("Sound"))
	{
		IUIWidgets::CreateComponentContentTitle("Sound Path");
		
		auto OldImagePos = ImGui::GetCursorPos();
		if (ImGui::Selectable("##SoundPicking", false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100)))
		{
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				std::string PathFound = ToolsEngine::OpenFileName();

				if (PathFound.empty() == false)
				{
					//Change sound here
					GetSoundComponentData().ChangeSound(PathFound);
				}
			}
		}
		ImGui::SetCursorPos(OldImagePos);
		ImGui::Image(m_textures[static_cast<int>(TextureNameComponentsWindow::BLACK_BACKGROUND)].GetTextureID(), ImVec2(100, 100));

		ImGui::SameLine();
		IUIWidgets::CreateText(GetSoundComponentData().GetPath().c_str());

		IUIWidgets::CreateComponentContentTitle("Looping");

		if (ImGui::Checkbox("##Looping", &GetSoundComponentData().IsLooping()))
		{
			m_bIsSoundUpdated = true;
		}

		IUIWidgets::CreateComponentContentTitle("Mute");

		if (ImGui::Checkbox("##Mute", &GetSoundComponentData().IsMute()))
		{
			m_bIsSoundUpdated = true;
		}

		IUIWidgets::CreateComponentContentTitle("Play on start");

		if (ImGui::Checkbox("##playonstart", &GetSoundComponentData().PlayOnStart()))
		{
			m_bIsSoundUpdated = true;
		}

		IUIWidgets::CreateComponentContentTitle("Volume");

		if (ImGui::InputFloat("##volume", &GetSoundComponentData().GetVolume()))
		{
			m_bIsSoundUpdated = true;
		}
		ImGui::TreePop();
	}
	IUIWidgets::OnRightClickComponent("Remove Sound Component", m_componentNameToRemove);
}

void libgfx_API UIWindowComponents::ChangeCurrentMeshTexturePreview(std::string path)
{
	if (path.empty() == false)
	{
		m_textures[static_cast<int>(TextureNameComponentsWindow::CURRENT_MESH_TEXTURE)].ChangeImage(DataTexture::CreateTexture(path, DataTexture::RGBA, false), m_textureSampler);
	}
}

void UIWindowComponents::RemoveActorDatasFromUI()
{
	m_ActorIDToDisplay = -1;
}

void UIWindowComponents::DoOnceForThisWindow()
{
	if (m_bDoOnce == true)
	{
		m_bDoOnce = false;

		CreateSampler();

		std::filesystem::path XAxisTexturePath = m_resourcesPath;

		XAxisTexturePath /= "UI/Icons/XAxis.png";

		std::filesystem::path YAxisTexturePath = m_resourcesPath;

		YAxisTexturePath /= "UI/Icons/YAxis.png";

		std::filesystem::path ZAxisTexturePath = m_resourcesPath;

		ZAxisTexturePath /= "UI/Icons/ZAxis.png";

		m_textures.emplace_back(DataTexture::CreateTexture(XAxisTexturePath.string(), DataTexture::RGBA, false), m_textureSampler);
		m_textures.emplace_back(DataTexture::CreateTexture(YAxisTexturePath.string(), DataTexture::RGBA, false), m_textureSampler);
		m_textures.emplace_back(DataTexture::CreateTexture(ZAxisTexturePath.string(), DataTexture::RGBA, false), m_textureSampler);

		std::filesystem::path BlackBackgroundTexturePath = m_resourcesPath;

		BlackBackgroundTexturePath /= "UI/Background/black.png";

		m_textures.emplace_back(DataTexture::CreateTexture(BlackBackgroundTexturePath.string(), DataTexture::RGBA, false), m_textureSampler);

		m_textures.emplace_back(DataTexture::CreateTexture(BlackBackgroundTexturePath.string(), DataTexture::RGBA, false), m_textureSampler);
	}
}

libgfx_API ActorTransformCompData& UIWindowComponents::GetTransformComponentData()
{
	return m_actorSelectedData.GetActorTransformCompData();
}

libgfx_API ActorStaticMeshCompData& UIWindowComponents::GetStaticMeshComponentData()
{
	return m_actorSelectedData.GetActorStaticMeshCompData();
}

libgfx_API ActorPhysicCompData& UIWindowComponents::GetPhysicComponentData()
{
	return m_actorSelectedData.GetActorPhysicCompData();
}

libgfx_API ActorCameraCompData& UIWindowComponents::GetCameraComponentData()
{
	return m_actorSelectedData.GetActorCameraCompData();
}

libgfx_API ActorLightCompData& UIWindowComponents::GetLightComponentData()
{
	return m_actorSelectedData.GetActorLightCompData();
}

libgfx_API ActorScriptCompData& UIWindowComponents::GetScriptComponentData()
{
	return m_actorSelectedData.GetActorScriptCompData();
}

libgfx_API ActorSoundCompData& UIWindowComponents::GetSoundComponentData()
{
	return m_actorSelectedData.GetActorSoundCompData();
}
