#pragma once

#include "UI/UIWindow.h"

#include "../../../../EngineEditor/sources/Scene/Scene.h"



class UIWindowWorldHierarchy : public UIWindow
{
public:
	libgfx_API UIWindowWorldHierarchy();
	//-------------------------------------------------------------------Other Functions--------------------------------------------------------------------
	//This function is called every frame to update the UI of this window
	void libgfx_API Update() override;


	//----------------------------------------------------------------This Window Related Functions------------------------------------------------------------------


	//TODO: Place this in parent class
	//This function will be executed if m_bDoOnce is = true, the idea is to use this function only once at the beginning of Update() to initialize stuff in this window only
	void DoOnceForThisWindow();

	void libgfx_API InitScene(Scene* scene);

	void DisplayRoot(Actor* actor);
	
	void DisplayChildren(Actor* actor);

	int libgfx_API GetActorSelectedID();
	void libgfx_API SetActorSelectedID(int ID);

	Actor* GetDragSourceActor(Actor* actor, unsigned int entityID);

	unsigned int libgfx_API GetFutureParentActorID();
	
	unsigned int libgfx_API GetFutureChildActorID();

	bool libgfx_API IsReparentingActors();

	void libgfx_API SetReparentingActors(bool tf);

	libgfx_API Delegate<void(std::string)>& GetCreateActorToRootDelegate();

	libgfx_API Delegate<void(Actor*, std::string)>& GetCreateActorToActorDelegate();

	
private:
	//TODO: Place this in parent class
	//This boolean is used to call DoOnceForThisWindow() only once, it's initialized at true at the launching of the app and should never be reset to true during runtime
	bool m_bDoOnce = true;

	Scene* m_scene;

	int m_node_clicked = -1;

	unsigned int m_futureParentActorID;
	unsigned int m_futureChildActorID;

	bool m_bReparentActors = false;

	char m_actorName[2048] = { 0 };

	bool m_bShouldCloseMainPopup = false;


	Delegate<void(std::string)> m_createActorToRootDelegate;
	Delegate<void(Actor*, std::string)> m_createActorToActorDelegate;

	
};
