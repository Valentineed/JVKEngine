

set(Engine_SOURCES
	sources/App.cpp
sources/App.h
sources/Components/Camera.cpp
sources/Components/Camera.h
sources/Components/Collider.cpp
sources/Components/Collider.h
sources/Components/Lights/DirectionalLight.cpp
sources/Components/Lights/DirectionalLight.h
sources/Components/Lights/LightComponent.cpp
sources/Components/Lights/LightComponent.h
sources/Components/Lights/PointLight.cpp
sources/Components/Lights/PointLight.h
sources/Components/Lights/SpotLight.cpp
sources/Components/Lights/SpotLight.h
sources/Components/ScriptComponent.cpp
sources/Components/ScriptComponent.h
sources/Components/SoundComponent.cpp
sources/Components/SoundComponent.h
sources/Components/StaticMesh.cpp
sources/Components/StaticMesh.h
sources/Components/Transform.cpp
sources/Components/Transform.h
sources/ECS/CommonTypes.h
sources/ECS/ComponentArray.h
sources/ECS/ComponentManager.cpp
sources/ECS/ComponentManager.h
sources/ECS/EntityManager.cpp
sources/ECS/EntityManager.h
sources/ECS/System.h
sources/ECS/SystemManager.cpp
sources/ECS/SystemManager.h
sources/ECS/World.cpp
sources/ECS/World.h
sources/InputSystem/DefineInput.h
sources/InputSystem/InputManager.cpp
sources/InputSystem/InputManager.h
sources/main.cpp
sources/Resources/ComponentsDataManager.cpp
sources/Resources/ComponentsDataManager.h
sources/Resources/ResourcesManager.cpp
sources/Resources/ResourcesManager.h
sources/Resources/ScriptObject.cpp
sources/Resources/ScriptObject.h
sources/Resources/TextureFactory.h
sources/Resources/ThreadPool.cpp
sources/Resources/ThreadPool.h
sources/Resources/ThreadPool.inl
sources/Scene/Actor.cpp
sources/Scene/Actor.h
sources/Scene/Scene.cpp
sources/Scene/Scene.h
sources/Scene/SceneManager.cpp
sources/Scene/SceneManager.h
sources/Scripting/CharacterScript.cpp
sources/Scripting/CharacterScript.h
sources/Scripting/ScriptObj.cpp
sources/Scripting/ScriptObj.h
sources/Serialization/Serializer.cpp
sources/Serialization/Serializer.h
sources/StateMachine/FSM.cpp
sources/StateMachine/FSM.h
sources/StateMachine/State.h
sources/StateMachine/States/Engine/StateEngine.cpp
sources/StateMachine/States/Engine/StateEngine.h
sources/StateMachine/States/Engine/Systems/CameraControlSystem.cpp
sources/StateMachine/States/Engine/Systems/CameraControlSystem.h
sources/StateMachine/States/Play/StatePlay.cpp
sources/StateMachine/States/Play/StatePlay.h
sources/Systems/CameraSystem.cpp
sources/Systems/CameraSystem.h
sources/Systems/LightSystem.cpp
sources/Systems/LightSystem.h
sources/Systems/PhysixSystem.cpp
sources/Systems/PhysixSystem.h
sources/Systems/RenderSystem.cpp
sources/Systems/RenderSystem.h
sources/Systems/ScriptingSystem.cpp
sources/Systems/ScriptingSystem.h
sources/Systems/SoundSystem.cpp
sources/Systems/SoundSystem.h
sources/Systems/StaticMeshSystem.cpp
sources/Systems/StaticMeshSystem.h
sources/Systems/TransformSystem.cpp
sources/Systems/TransformSystem.h
sources/TestGameplay/Bullet.cpp
sources/TestGameplay/Bullet.h
sources/TestGameplay/Bullet2.cpp
sources/TestGameplay/Bullet2.h
sources/TestGameplay/Button.cpp
sources/TestGameplay/Button.h
sources/TestGameplay/Capsule.cpp
sources/TestGameplay/Capsule.h
sources/TestGameplay/CustomActor.cpp
sources/TestGameplay/CustomActor.h
sources/TestGameplay/DeathBox.cpp
sources/TestGameplay/DeathBox.h
sources/TestGameplay/Floor.cpp
sources/TestGameplay/Floor.h
sources/TestGameplay/LvlManager.cpp
sources/TestGameplay/LvlManager.h
sources/TestGameplay/Mountain.cpp
sources/TestGameplay/Mountain.h
sources/TestGameplay/PickUpObject.cpp
sources/TestGameplay/PickUpObject.h
sources/TestGameplay/Platform.cpp
sources/TestGameplay/Platform.h
sources/TestGameplay/PlatformSpawner.cpp
sources/TestGameplay/PlatformSpawner.h
sources/TestGameplay/Portal.cpp
sources/TestGameplay/Portal.h
sources/TestGameplay/Portal2.cpp
sources/TestGameplay/Portal2.h
sources/TestGameplay/StaticBox.cpp
sources/TestGameplay/StaticBox.h
sources/TestGameplay/Trampoline.cpp
sources/TestGameplay/Trampoline.h
sources/TestGameplay/Trap.cpp
sources/TestGameplay/Trap.h
	)
