#include "Serializer.h"
#include "Matrix/Matrix4.h"
#include "Vector/Vector.h"
#include "../Scene/Scene.h"
#include "../Scene/Actor.h"
#include "../Components/Transform.h"

#include <tinyxml.h>
#include <iostream>

#include "RenderingManager.h"
#include "Shapes/Primitives/BoxShape.h"
#include "Shapes/Primitives/CapsuleShape.h"
#include "Shapes/Primitives/SphereShape.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/CameraSystem.h"
#include "../Systems/SoundSystem.h"
#include "../TestGameplay/Button.h"
#include "../TestGameplay/Capsule.h"
#include "../TestGameplay/CustomActor.h"
#include "../TestGameplay/DeathBox.h"
#include "../TestGameplay/Mountain.h"
#include "../TestGameplay/Trampoline.h"
#include "../TestGameplay/LvlManager.h"
#include "../TestGameplay/StaticBox.h"
#include "../TestGameplay/PickUpObject.h"
#include "../TestGameplay/Platform.h"
#include "../TestGameplay/Floor.h"
#include "../TestGameplay/PlatformSpawner.h"
#include "../TestGameplay/Trap.h"
#include "UI/UIWidgetImage.h"
#include "UI/UIWidgetText.h"


namespace XML
{
	Serializer& Serializer::GetInstance()
	{
		static Serializer newInstance;

		return newInstance;
	}

	void Serializer::ExportScene(Scene* scene)
	{
		World& world = World::GetInstance();
		TiXmlDocument doc;
		TiXmlComment* comment;
		std::string s;
		std::stringstream ss;
		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
		doc.LinkEndChild(decl);

		TiXmlElement* root = new TiXmlElement(scene->m_name.data());
		doc.LinkEndChild(root);

		comment = new TiXmlComment();
		s = " Settings for scene : " + static_cast<std::string>(scene->m_name.data()) + " ";
		comment->SetValue(s.c_str());
		root->LinkEndChild(comment);

		{
			TiXmlElement* rootActor = new TiXmlElement("RootActor");
			root->LinkEndChild(rootActor);
			ss = MatrixToText(scene->m_root->GetTransform()->m_localTransform);

			rootActor->SetAttribute("RootName", scene->m_root->m_name.c_str());
			rootActor->SetAttribute("EntityID", scene->m_root->m_idEntity);
			rootActor->SetAttribute("RootLocalTrans", ss.str().c_str());
			int id = world.GetSystem<RenderSystem>()->m_renderingMgr->m_UIWindowManager.GetActorIDToPossess();
			rootActor->SetAttribute("CameraIndex", id);

			SaveRecursive(rootActor, *scene->m_root);
		}

		{

			for (auto& widget : world.GetSystem<RenderSystem>()->GetUIWindow().GetWidgets())
			{
				TiXmlElement* UI = new TiXmlElement("UIWidget");
				root->LinkEndChild(UI);
				
				UI->SetAttribute("WidgetType", (int)widget->GetType());
				UI->SetDoubleAttribute("WidgetPosX", widget->GetPos().x);
				UI->SetDoubleAttribute("WidgetPosY", widget->GetPos().y);
				UI->SetAttribute("WidgetAnchor", (int)widget->GetAnchor());
				UI->SetDoubleAttribute("WidgetAnchorPositionX", widget->GetAnchorPosition().x);
				UI->SetDoubleAttribute("WidgetAnchorPositionY", widget->GetAnchorPosition().y);
				UI->SetDoubleAttribute("WidgetAlignmentX", widget->GetAlignment().y);
				UI->SetDoubleAttribute("WidgetAlignmentY", widget->GetAlignment().y);

				if (widget->GetType() == WidgetsType::IMAGE)
				{
					UI->SetDoubleAttribute("WidgetSizeX", static_cast<UIWidgetImage*>(widget.get())->GetSize().x);
					UI->SetDoubleAttribute("WidgetSizeY", static_cast<UIWidgetImage*>(widget.get())->GetSize().y);
					UI->SetAttribute("WidgetImagePath", static_cast<UIWidgetImage*>(widget.get())->GetImagePath().c_str());
				}
				else if (widget->GetType() == WidgetsType::TEXT)
				{
					UI->SetAttribute("WidgetText", static_cast<UIWidgetText*>(widget.get())->m_bufferText);
				}
				
			}
		}
		std::string save;
		if (m_IfOnDefaultMap == true)
		{
			save.append("../");
			save.append(m_currentScenePath);
		}
		else
		{
			//save.append(m_currentScenePath);]
			auto count = m_currentScenePath.find("Assets\\");
			m_currentScenePath.erase(0, count);
			save.append("../");
			save.append(m_currentScenePath);
			
		}
		//std::string save = scene->m_name.data();
		//save += ".xml";
		if (doc.SaveFile(save.data()))
		{
			printf("SCENE SAVED SUCCESSFULLY\n");
		}
	}

	void Serializer::SaveRecursive(TiXmlElement* parent, Actor& actor)
	{
		if (actor.m_children.empty())
			return;

		std::string s;
		std::stringstream ss;
		for (int i = 0; i < actor.m_children.size(); i++)
		{
			TiXmlElement* child;
			s.clear();
			s = "Child";
			//s += std::to_string(i);

			child = new TiXmlElement(s.c_str());
			parent->LinkEndChild(child);

			ss.clear();
			ss = MatrixToText(actor.m_children[i]->GetTransform()->m_localTransform);

			child->SetAttribute("Name", actor.m_children[i]->m_name.c_str());
			child->SetAttribute("LocalTrans", ss.str().c_str());
			if (actor.m_children[i]->m_name == "CustomActor" || actor.m_children[i]->m_bIsOG)
			{
				if (actor.m_children[i]->GetMeshComponent() != nullptr)
				{
					child->SetAttribute("StaticMesh", 1);
					s.clear();
					s = actor.m_children[i]->GetMeshComponent()->m_currentMeshPath;
					child->SetAttribute("ObjName", s.c_str());
					s.clear();
					s = actor.m_children[i]->GetMeshComponent()->m_currentTexturePath;
					child->SetAttribute("TextureName", s.c_str());
				}
				auto* physicsComponent = actor.m_children[i]->GetPhysicsComponent();
				if (physicsComponent != nullptr)
				{
					child->SetAttribute("Collider", 1);

					child->SetAttribute("RigidType", static_cast<int>(actor.m_children[i]->GetPhysicsComponent()->collisionType));
					child->SetAttribute("Scale", Vector3ToText(actor.m_children[i]->GetPhysicsComponent()->m_scale).str().c_str());
					child->SetAttribute("bKinematic", actor.m_children[i]->GetPhysicsComponent()->m_bIsKinematic);
					child->SetAttribute("bGravity", actor.m_children[i]->GetPhysicsComponent()->m_bIsGravityDisabled);
					child->SetAttribute("bTrigger", actor.m_children[i]->GetPhysicsComponent()->m_bIsTrigger);
					child->SetAttribute("bQuery", actor.m_children[i]->GetPhysicsComponent()->m_bIsQuery);
					child->SetAttribute("bIntersection", actor.m_children[i]->GetPhysicsComponent()->m_bHaveIntersection);
					child->SetAttribute("bLockAngX", actor.m_children[i]->GetPhysicsComponent()->m_lockAngularX);
					child->SetAttribute("bLockAngY", actor.m_children[i]->GetPhysicsComponent()->m_lockAngularY);
					child->SetAttribute("bLockAngZ", actor.m_children[i]->GetPhysicsComponent()->m_lockAngularZ);
					child->SetAttribute("bLockLinX", actor.m_children[i]->GetPhysicsComponent()->m_lockLinearX);
					child->SetAttribute("bLockLinY", actor.m_children[i]->GetPhysicsComponent()->m_lockLinearY);
					child->SetAttribute("bLockLinZ", actor.m_children[i]->GetPhysicsComponent()->m_lockLinearZ);

					const auto dim = physicsComponent->GetScaleShape();
					if (dim.z != (float)INVALID_INDEX)
					{
						child->SetAttribute("Shape", "Box");
						child->SetDoubleAttribute("xExtent", dim.x);
						child->SetDoubleAttribute("yExtent", dim.y);
						child->SetDoubleAttribute("zExtent", dim.z);
					}
					else
					{
						if (dim.y != (float)INVALID_INDEX)
						{
							child->SetAttribute("Shape", "Capsule");
							child->SetDoubleAttribute("Radius", dim.x);
							child->SetDoubleAttribute("HalfHeight", dim.y);
						}
						else
						{
							child->SetAttribute("Shape", "Sphere");
							child->SetDoubleAttribute("Radius", dim.x);
						}
					}
				}
				if (actor.m_children[i]->GetCameraComponent() != nullptr)
				{
					child->SetAttribute("Camera", 1);
					ss.clear();
					ss = Vector3ToText(actor.m_children[i]->GetCameraComponent()->m_OffsetFromPlayer);
					child->SetAttribute("Offset", ss.str().c_str());
				}
				if (actor.m_children[i]->GetLightComponent<PointLight>() != nullptr && actor.m_children[i]->GetLightComponent<PointLight>()->type == Type::POINT)
				{
					child->SetAttribute("PointLight", 1);
					ss.clear();
					ss = Vector4ToText(actor.m_children[i]->GetLightComponent<PointLight>()->GetPosition());
					child->SetAttribute("Position", ss.str().c_str());
					ss.clear();
					ss = Vector3ToText(actor.m_children[i]->GetLightComponent<PointLight>()->GetAmbientColor());
					child->SetAttribute("Ambient", ss.str().c_str());
					ss.clear();
					ss = Vector3ToText(actor.m_children[i]->GetLightComponent<PointLight>()->GetDiffuseColor());
					child->SetAttribute("Diffuse", ss.str().c_str());
					ss.clear();
					ss = Vector3ToText(actor.m_children[i]->GetLightComponent<PointLight>()->GetSpecularColor());
					child->SetAttribute("Specular", ss.str().c_str());
					child->SetDoubleAttribute("kConstant", actor.m_children[i]->GetLightComponent<PointLight>()->GetConstantAttenuation());
					child->SetDoubleAttribute("kLinear", actor.m_children[i]->GetLightComponent<PointLight>()->GetLinearAttenuation());
					child->SetDoubleAttribute("kQuad", actor.m_children[i]->GetLightComponent<PointLight>()->GetQuadraticAttenuation());
				}
				if (actor.m_children[i]->GetLightComponent<DirectionalLight>() != nullptr && actor.m_children[i]->GetLightComponent<DirectionalLight>()->type == Type::DIRECTIONAL)
				{
					child->SetAttribute("DirLight", 1);
					ss.clear();
					ss = Vector4ToText(actor.m_children[i]->GetLightComponent<DirectionalLight>()->GetPosition());
					child->SetAttribute("Direction", ss.str().c_str());
					ss.clear();
					ss = Vector3ToText(actor.m_children[i]->GetLightComponent<DirectionalLight>()->GetAmbientColor());
					child->SetAttribute("Ambient", ss.str().c_str());
					ss.clear();
					ss = Vector3ToText(actor.m_children[i]->GetLightComponent<DirectionalLight>()->GetDiffuseColor());
					child->SetAttribute("Diffuse", ss.str().c_str());
					ss.clear();
					ss = Vector3ToText(actor.m_children[i]->GetLightComponent<DirectionalLight>()->GetSpecularColor());
					child->SetAttribute("Specular", ss.str().c_str());
				}
				if (actor.m_children[i]->GetLightComponent<SpotLight>() != nullptr && actor.m_children[i]->GetLightComponent<SpotLight>()->type == Type::SPOT)
				{
					child->SetAttribute("SpotLight", 1);
					ss.clear();
					ss = Vector4ToText(actor.m_children[i]->GetLightComponent<SpotLight>()->GetPosition());
					child->SetAttribute("Position", ss.str().c_str());
					ss.clear();
					ss = Vector3ToText(actor.m_children[i]->GetLightComponent<SpotLight>()->GetDirection());
					child->SetAttribute("Direction", ss.str().c_str());
					child->SetDoubleAttribute("Cutoff", actor.m_children[i]->GetLightComponent<SpotLight>()->GetCutOff());
					child->SetDoubleAttribute("OutCutoff", actor.m_children[i]->GetLightComponent<SpotLight>()->GetOuterCutOff());
					ss.clear();
					ss = Vector3ToText(actor.m_children[i]->GetLightComponent<SpotLight>()->GetAmbientColor());
					child->SetAttribute("Ambient", ss.str().c_str());
					ss.clear();
					ss = Vector3ToText(actor.m_children[i]->GetLightComponent<SpotLight>()->GetDiffuseColor());
					child->SetAttribute("Diffuse", ss.str().c_str());
					ss.clear();
					ss = Vector3ToText(actor.m_children[i]->GetLightComponent<SpotLight>()->GetSpecularColor());
					child->SetAttribute("Specular", ss.str().c_str());
					child->SetDoubleAttribute("kConstant", actor.m_children[i]->GetLightComponent<SpotLight>()->GetConstantAttenuation());
					child->SetDoubleAttribute("kLinear", actor.m_children[i]->GetLightComponent<SpotLight>()->GetLinearAttenuation());
					child->SetDoubleAttribute("kQuad", actor.m_children[i]->GetLightComponent<SpotLight>()->GetQuadraticAttenuation());
				}
				if (actor.m_children[i]->GetSoundComponent() != nullptr)
				{
					child->SetAttribute("Sound", 1);
					child->SetAttribute("Path", actor.m_children[i]->GetSoundComponent()->GetPath().c_str());
					child->SetDoubleAttribute("Volume", actor.m_children[i]->GetSoundComponent()->m_volume);
					child->SetDoubleAttribute("Frequency", actor.m_children[i]->GetSoundComponent()->m_frequency);
					child->SetAttribute("bLooping", actor.m_children[i]->GetSoundComponent()->m_bIsLooping);
					child->SetAttribute("bMute", actor.m_children[i]->GetSoundComponent()->m_bIsMute);
					child->SetAttribute("bPonWake", actor.m_children[i]->GetSoundComponent()->m_bPlayOnWake);
				}

				child->SetAttribute("IsPossessed", actor.m_bIsPossessed);
			}

			SaveRecursive(child, *actor.m_children[i]);
		}
	}

	Scene Serializer::LoadScene(const char* filename)
	{
		World& world = World::GetInstance();
		Scene newScene;

		TiXmlDocument doc(filename);
		if (!doc.LoadFile())
		{
			printf("No file existing with this name\n");
			return newScene;
		}

		m_currentScenePath = filename;
		TiXmlHandle hDoc(&doc);
		TiXmlElement* pElem;
		TiXmlHandle hRoot(0);

		pElem = hDoc.FirstChildElement().Element();
		if (!pElem)
			return newScene;

		newScene.m_name = pElem->Value();
		hRoot = TiXmlHandle(pElem);
		{
			pElem = hRoot.FirstChild("RootActor").Element();
			Actor* rootActor = new Actor;
			Transform rootTrans;
			Entity rootEn;
			rootActor->m_name = pElem->Attribute("RootName");
			rootEn = std::atoi(pElem->Attribute("EntityID"));
			rootTrans.m_localTransform = TextToMatrix(pElem->Attribute("RootLocalTrans"));
			rootActor->m_idEntity = rootEn;
			rootActor->SetTransform(rootTrans);
			newScene.m_root = rootActor;
			if (pElem->Attribute("CameraIndex") != nullptr)
			{
				std::stringstream strValue(pElem->Attribute("CameraIndex"));
				int camID;
				strValue >> camID;
				world.GetSystem<RenderSystem>()->m_renderingMgr->m_UIWindowManager.SetActorIDToPossess(camID);
			}

			TiXmlElement* pRoot = hRoot.FirstChildElement().Element();
			LoadRecursive(pRoot, *rootActor, newScene);

		}

		{
			if(world.GetSystem<RenderSystem>()->GetUIWindow().GetWidgets().empty() == false)
			{
				world.GetSystem<RenderSystem>()->GetUIWindow().ClearWidgets();
			}
			
			if (hRoot.FirstChild("UIWidget").ToElement() != nullptr)
			{
				pElem = hRoot.FirstChild("UIWidget").ToElement();

				if (std::atoi(pElem->Attribute("WidgetType")) == (int)WidgetsType::IMAGE)
				{
					world.GetSystem<RenderSystem>()->GetUIWindow().AddWidgetImage(
						pElem->Attribute("WidgetImagePath"),
						(Anchors)std::atoi(pElem->Attribute("WidgetAnchor")),
						ImVec2(std::atof(pElem->Attribute("WidgetAnchorPositionX")), std::atof(pElem->Attribute("WidgetAnchorPositionY"))),
						ImVec2(std::atof(pElem->Attribute("WidgetAlignmentX")), std::atof(pElem->Attribute("WidgetAlignmentY"))),
						ImVec2(std::atof(pElem->Attribute("WidgetSizeX")), std::atof(pElem->Attribute("WidgetSizeY"))),
						ImVec2(std::atof(pElem->Attribute("WidgetPosX")), std::atof(pElem->Attribute("WidgetPosY")))
					);
				}
				else if (std::atoi(pElem->Attribute("WidgetType")) == (int)WidgetsType::TEXT)
				{
					world.GetSystem<RenderSystem>()->GetUIWindow().AddWidgetText(
						pElem->Attribute("WidgetText"),
						(Anchors)std::atoi(pElem->Attribute("WidgetAnchor")),
						ImVec2(std::atof(pElem->Attribute("WidgetAnchorPositionX")), std::atof(pElem->Attribute("WidgetAnchorPositionY"))),
						ImVec2(std::atof(pElem->Attribute("WidgetAlignmentX")), std::atof(pElem->Attribute("WidgetAlignmentY"))),
						ImVec2(std::atof(pElem->Attribute("WidgetPosX")), std::atof(pElem->Attribute("WidgetPosY")))
					);
				}
				
				

				while(pElem->NextSibling() != nullptr)
				{
					pElem = pElem->NextSibling()->ToElement();
					if (std::atoi(pElem->Attribute("WidgetType")) == (int)WidgetsType::IMAGE)
					{
						world.GetSystem<RenderSystem>()->GetUIWindow().AddWidgetImage(
							pElem->Attribute("WidgetImagePath"),
							(Anchors)std::atoi(pElem->Attribute("WidgetAnchor")),
							ImVec2(std::atof(pElem->Attribute("WidgetAnchorPositionX")), std::atof(pElem->Attribute("WidgetAnchorPositionY"))),
							ImVec2(std::atof(pElem->Attribute("WidgetAlignmentX")), std::atof(pElem->Attribute("WidgetAlignmentY"))),
							ImVec2(std::atof(pElem->Attribute("WidgetSizeX")), std::atof(pElem->Attribute("WidgetSizeY"))),
							ImVec2(std::atof(pElem->Attribute("WidgetPosX")), std::atof(pElem->Attribute("WidgetPosY")))
						);
					}
					else if (std::atoi(pElem->Attribute("WidgetType")) == (int)WidgetsType::TEXT)
					{
						world.GetSystem<RenderSystem>()->GetUIWindow().AddWidgetText(
							pElem->Attribute("WidgetText"),
							(Anchors)std::atoi(pElem->Attribute("WidgetAnchor")),
							ImVec2(std::atof(pElem->Attribute("WidgetAnchorPositionX")), std::atof(pElem->Attribute("WidgetAnchorPositionY"))),
							ImVec2(std::atof(pElem->Attribute("WidgetAlignmentX")), std::atof(pElem->Attribute("WidgetAlignmentY"))),
							ImVec2(std::atof(pElem->Attribute("WidgetPosX")), std::atof(pElem->Attribute("WidgetPosY")))
						);
					}
					
				}
			}
			world.GetSystem<RenderSystem>()->GetUIWindow().SetNeedReloadViewportWidgets(true);
		}

		return newScene;
	}

	void Serializer::LoadRecursive(TiXmlNode* pParent, Actor& parentActor, Scene& sceneToFill)
	{
		TiXmlElement* pChild = pParent->FirstChildElement();

		for (pChild; pChild; pChild = pChild->NextSiblingElement())
		{
			m_bCustom = false;
			if (pChild->Attribute("Name") == std::string("CustomActor") || pChild->Attribute("Name") == std::string("Actor"))

				parentActor.AddChild(GetActorType(pChild->Attribute("Name"), std::make_pair(pChild->Attribute("ObjPath"), pChild->Attribute("TexturePath"))));
			else
				parentActor.AddChild(GetActorType(pChild->Attribute("Name")));

			Actor& child = *parentActor.m_children.back();
			Transform childTrans;
			child.m_name = pChild->Attribute("Name");
			childTrans.m_localTransform = TextToMatrix(pChild->Attribute("LocalTrans"));
			child.SetTransform(childTrans);

			if (m_bCustom)
			{
				if (pChild->Attribute("StaticMesh") != nullptr)
				{
					//std::string objPath = "resources\\models\\";
					std::string objPath = pChild->Attribute("ObjName");
					//std::string texturePath = "resources\\images\\";
					std::string texturePath = pChild->Attribute("TextureName");
					size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(child.GetTransform()->myIndexSystem, objPath, texturePath);
					child.AddComponent<StaticMesh>(indexMesh);
				}
				if (pChild->Attribute("Collider") != nullptr)
				{
					Collider collider;
					RigidType rType;
					if (pChild->Attribute("RigidType") == std::string("0"))
						rType = RigidType::DYNAMIC;
					if (pChild->Attribute("RigidType") == std::string("1"))
						rType = RigidType::STATIC;
					if (pChild->Attribute("RigidType") == std::string("2"))
						rType = RigidType::PLANE;

					if (pChild->Attribute("Shape") == std::string("Box"))
					{
						BoxShape* shape = new BoxShape(TextTofloat(pChild->Attribute("xExtent")),
							TextTofloat(pChild->Attribute("yExtent")),
							TextTofloat(pChild->Attribute("zExtent")));
						collider = Collider(*shape, rType, *child.GetTransform(), child.m_idEntity);
					}
					else if (pChild->Attribute("Shape") == std::string("Capsule"))
					{
						CapsuleShape* shape = new CapsuleShape(TextTofloat(pChild->Attribute("Radius")), TextTofloat(pChild->Attribute("HalfHeight")));
						collider = Collider(*shape, rType, *child.GetTransform(), child.m_idEntity);
					}
					else if (pChild->Attribute("Shape") == std::string("Sphere"))
					{
						SphereShape* shape = new SphereShape(TextTofloat(pChild->Attribute("Radius")));
						collider = Collider(*shape, rType, *child.GetTransform(), child.m_idEntity);
					}

					collider.SetScale(TextToVec3(pChild->Attribute("Scale")));
					collider.m_bIsKinematic = pChild->Attribute("bKinematic") == std::string("0") ? false : true;
					collider.m_bIsGravityDisabled = pChild->Attribute("bGravity") == std::string("0") ? false : true;
					collider.m_bIsTrigger = pChild->Attribute("bTrigger") == std::string("0") ? false : true;
					collider.m_bIsQuery = pChild->Attribute("bQuery") == std::string("0") ? false : true;
					collider.m_bHaveIntersection = pChild->Attribute("bIntersection") == std::string("0") ? false : true;
					collider.m_lockAngularX = pChild->Attribute("bLockAngX") == std::string("0") ? false : true;
					collider.m_lockAngularY = pChild->Attribute("bLockAngY") == std::string("0") ? false : true;
					collider.m_lockAngularZ = pChild->Attribute("bLockAngZ") == std::string("0") ? false : true;
					collider.m_lockLinearX = pChild->Attribute("bLockLinX") == std::string("0") ? false : true;
					collider.m_lockLinearY = pChild->Attribute("bLockLinY") == std::string("0") ? false : true;
					collider.m_lockLinearZ = pChild->Attribute("bLockLinZ") == std::string("0") ? false : true;
					
					size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(collider);
					child.AddComponent<Collider>(indexCollider);
				}
				if (pChild->Attribute("Camera") != nullptr)
				{
					size_t indexCamera = World::GetInstance().GetSystem<CameraSystem>()->Register(child.GetTransform()->GetLocalLocation(), { 0.f, 0.f, -1.f }, { 0.f, 1.f, 0.f });
					child.AddComponent<Camera>(indexCamera);
					child.GetCameraComponent()->m_OffsetFromPlayer = TextToVec3(pChild->Attribute("Offset"));
				}
				if (pChild->Attribute("PointLight") != nullptr)
				{
					PointLight pointLight(TextToVec3(pChild->Attribute("Position")),
						TextToVec3(pChild->Attribute("Ambient")),
						TextToVec3(pChild->Attribute("Diffuse")),
						TextToVec3(pChild->Attribute("Specular")),
						TextTofloat(pChild->Attribute("kConstant")),
						TextTofloat(pChild->Attribute("kLinear")),
						TextTofloat(pChild->Attribute("kQuad")));

					size_t indexLight = World::GetInstance().GetSystem<LightSystem>()->Register(pointLight);
					child.AddComponent<LightComponent>(indexLight);
				}
				if (pChild->Attribute("DirLight") != nullptr)
				{
					DirectionalLight dirLight(TextToVec3(pChild->Attribute("Direction")),
						TextToVec3(pChild->Attribute("Ambient")),
						TextToVec3(pChild->Attribute("Diffuse")),
						TextToVec3(pChild->Attribute("Specular")));

					size_t indexLight = World::GetInstance().GetSystem<LightSystem>()->Register(dirLight);
					child.AddComponent<LightComponent>(indexLight);
				}
				if (pChild->Attribute("SpotLight") != nullptr)
				{
					SpotLight spotLight(child.GetTransform()->GetLocalLocation(),
						TextToVec3(pChild->Attribute("Direction")),
						TextTofloat(pChild->Attribute("Cutoff")),
						TextTofloat(pChild->Attribute("OutCutoff")),
						TextToVec3(pChild->Attribute("Ambient")),
						TextToVec3(pChild->Attribute("Diffuse")),
						TextToVec3(pChild->Attribute("Specular")),
						TextTofloat(pChild->Attribute("kConstant")),
						TextTofloat(pChild->Attribute("kLinear")),
						TextTofloat(pChild->Attribute("kQuad")));

					size_t indexLight = World::GetInstance().GetSystem<LightSystem>()->Register(spotLight);
					child.AddComponent<LightComponent>(indexLight);
				}
				if (pChild->Attribute("Sound") != nullptr)
				{
					size_t indexSound = World::GetInstance().GetSystem<SoundSystem>()->Register(pChild->Attribute("Path"), child.GetTransform()->GetLocalLocation(), 10, 5);
					child.AddComponent<SoundComponent>(indexSound);
					child.GetSoundComponent()->m_volume = TextTofloat(pChild->Attribute("Volume"));
					child.GetSoundComponent()->m_volume = TextTofloat(pChild->Attribute("Frequency"));
					child.GetSoundComponent()->m_bIsLooping = pChild->Attribute("bLooping") == std::string("0") ? false : true;
					child.GetSoundComponent()->m_bIsMute = pChild->Attribute("bMute") == std::string("0") ? false : true;
					child.GetSoundComponent()->m_bPlayOnWake = pChild->Attribute("bPonWake") == std::string("0") ? false : true;
				}
			}
			child.SetParent(parentActor);

			if (pChild->FirstChildElement() != nullptr)
			{
				LoadRecursive(pChild, *sceneToFill.m_root->m_children.back(), sceneToFill);
			}
		}
	}

	float Serializer::TextTofloat(const char* text)
	{
		float f;
		std::stringstream ss(text);
		ss >> f;

		return f;
	}

	std::stringstream Serializer::MatrixToText(Matrix4& mat)
	{
		std::stringstream ss;// (std::stringstream::in);
		std::string str;

		for (int i = 0; i < 16; i++)
		{
			str = std::to_string(mat.raw[i]);

			if (i != 15)
				str += " ";

			ss << str;
			str.clear();
		}

		return ss;
	}

	std::stringstream Serializer::Vector3ToText(const Vector3& vec)
	{
		std::stringstream ss;

		ss << vec.x << " ";
		ss << vec.y << " ";
		ss << vec.z;

		return ss;
	}

	Vector3 Serializer::TextToVec3(const char* text)
	{
		Vector3 vec;
		std::stringstream ss(text);

		ss >> vec.x;
		ss >> vec.y;
		ss >> vec.z;

		return vec;
	}

	std::stringstream Serializer::Vector4ToText(const LibMath::Vector4& vec)
	{
		std::stringstream ss;

		ss << vec.x << " ";
		ss << vec.y << " ";
		ss << vec.z << " ";
		ss << vec.w;

		return ss;
	}

	Vector4 Serializer::TextToVec4(const char* text)
	{
		Vector4 vec;
		std::stringstream ss(text);

		ss >> vec.x;
		ss >> vec.y;
		ss >> vec.z;
		ss >> vec.w;

		return vec;
	}

	Matrix4 Serializer::TextToMatrix(const char* text)
	{
		Matrix4 mat;
		std::stringstream ss(text);

		for (int i = 0; i < 16; i++)
		{
			float val;
			ss >> val;
			mat.raw[i] = val;
		}

		return mat;
	}

	Actor* Serializer::GetActorType(const char* text, std::pair<std::string, std::string> paths)
	{
		Actor* newActor;

		if (text == std::string("CustomActor"))
		{
			newActor = new CustomActor{ paths.first, paths.second };
			m_bCustom = true;
		}
		else if (text == std::string("Trampoline"))
			newActor = new Trampoline{};
		else if (text == std::string("PickUpObject"))
			newActor = new PickUpObject{};
		else if (text == std::string("StaticBox"))
			newActor = new StaticBox{};
		else if (text == std::string("Ground"))
			newActor = new Mountain{};
		else if (text == std::string("LvlManager"))
			newActor = new LvlManager{};
		else if (text == std::string("Capsule"))
			newActor = new Capsule{};
		else if (text == std::string("DeathBox"))
			newActor = new DeathBox{};
		else if (text == std::string("PlatformSpawner"))
			newActor = new PlatformSpawner{};
		else if (text == std::string("Platform"))
			newActor = new Platform{};
		else if (text == std::string("Trap"))
			newActor = new Trap{};
		else if (text == std::string("Button"))
			newActor = new Button{};
		else if (text == std::string("Floor"))
			newActor = new Floor{};
		else
		{
			newActor = new Actor;
			m_bCustom = true;
		}

		return newActor;
	}
}
