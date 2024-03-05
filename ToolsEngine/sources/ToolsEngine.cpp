#include "ToolsEngine.h"


#include <windows.h>
#include <commdlg.h>
#include <shlobj_core.h>
#include <fstream> 
#include <cstdio>
#include <filesystem>
#include <iostream>


void ToolsEngine::TestMessageToolsEngine()
{
	printf("Hello TestToolsEngine");
}

const std::string ToolsEngine::OpenFileName()
{
	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
	char szFile[1000] = { 0 };

	std::string assetspath = std::filesystem::current_path().string();

	assetspath += "\\Assets";

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.hwndOwner = NULL;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = ("Open\0");
	ofn.nFilterIndex = 0;
	ofn.lpstrInitialDir = assetspath.c_str();
	ofn.lpstrDefExt = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;
	ofn.lpstrFilter = ("*All Files\0*.*\0\0");

	if (GetOpenFileNameA(&ofn))
	{
		return ofn.lpstrFile;
	}

	return "";
}

void ToolsEngine::CreateScriptCharacter(std::string& scriptPath, std::string& scriptName)
{
	scriptName = scriptName.substr(0, scriptName.size() - 4);

	std::ofstream outfile(scriptPath.c_str());

	outfile << scriptName << " = Character:new()\n" << std::endl;
	outfile << "function " << scriptName << ":Start()" << std::endl;
	outfile << "end\n" << std::endl;

	outfile << "function " << scriptName << ":Update()" << std::endl;
	outfile << "end\n" << std::endl;

	outfile.close();
}

void ToolsEngine::ReadBatchFile(std::string_view fileName)
{
	std::filesystem::path p(fileName);
	std::system(p.string().c_str());
}
#pragma warning(push,  0)

INT CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	if (uMsg == BFFM_INITIALIZED) SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
	return 0;
}
#pragma warning(pop)



const std::string ToolsEngine::ChoseFolderPath()
{
	HWND m_hwnd = {};
	std::string PathReturn;

	BROWSEINFO br;
	ZeroMemory(&br, sizeof(BROWSEINFO));
	br.lpfn = BrowseCallbackProc;
	br.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	br.hwndOwner = m_hwnd;
	br.lpszTitle = "Chose a folder";
	/*br.lParam = (LPARAM)folder.GetString();*/

	LPITEMIDLIST pidl = NULL;
	if ((pidl = SHBrowseForFolder(&br)) != NULL)
	{
		char buffer[MAX_PATH];
		if (SHGetPathFromIDList(pidl, buffer)) PathReturn = buffer;
	}

	return PathReturn;
}

void ToolsEngine::RewriteBatShippingFile(std::string& dirPath)
{
	std::ofstream outfile("resources/ship.bat");

	outfile << "@echo Should copy necessary directories near .exe" << std::endl;
	outfile << "SET mypath=%~dp0\n" << std::endl;

	outfile << "@echo %mypath%\n" << std::endl;
	
	outfile << "call \"%ProgramFiles(x86)%\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\VsDevCmd.bat\"" << std::endl;
	
	outfile << "devenv %mypath%/../Build/JVKEngine.sln /build Shipping" << std::endl;

	outfile << "robocopy %mypath%/../build/bin/Shipping " << dirPath << "/JVKEngine" << " /E /mt /z" << std::endl;
	outfile << "robocopy %mypath%/ " << dirPath << "/JVKEngine/resources" << " /E /mt /z" << std::endl;
	outfile << "robocopy %mypath%/../Assets/ " << dirPath << "/JVKEngine/Assets" << " /E /mt /z" << std::endl;
	outfile << "\npause" << std::endl;

	outfile.close();

}

void ToolsEngine::CreateNewLevel(std::string& levelName)
{
	std::string path = "Assets/Levels/";

	path += levelName.c_str();
	path += ".xml";
	std::ofstream outfile(path.c_str());

	outfile << "<?xml version=\"1.0\" ?>" << std::endl;
	outfile << "<" << levelName.c_str() << ">" << std::endl;
	outfile << "    <!-- Settings for scene : " << levelName.c_str() << " -->" << std::endl;
	outfile << "    <RootActor RootName=\"[Main Root]\" EntityID=\"0\" RootLocalTrans=\"1.000000 0.000000 0.000000 0.000000 0.000000 1.000000 0.000000 0.000000 0.000000 0.000000 1.000000 0.000000 0.000000 0.000000 0.000000 1.000000\" CameraIndex=\"2\">" << std::endl;
	outfile << "        <Child Name=\"Floor\" LocalTrans=\"2.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 2.000000 0.000000 0.000000 -2.000000 0.000000 1.000000\" StaticMesh=\"1\" ObjName=\"Assets/StarterContent/Floor.obj\" TextureName=\"resources/images/missing-texture.png\" Collider=\"1\" RigidType=\"-858993460\" Scale=\"1 1 1\" bKinematic=\"0\" bGravity=\"0\" bTrigger=\"0\" bQuery=\"0\" bIntersection=\"1\" bLockAngX=\"0\" bLockAngY=\"0\" bLockAngZ=\"0\" bLockLinX=\"0\" bLockLinY=\"0\" bLockLinZ=\"0\" Shape=\"Sphere\" Radius=\"-2147483648\" DirLight=\"1\" Direction=\"0 1 0 0\" Ambient=\"0.7 0.7 0.7\" Diffuse=\"0.7 0.7 0.7\" Specular=\"0.7 0.7 0.7\" IsPossessed=\"0\" />" << std::endl;


	outfile << "    </RootActor>" << std::endl;
	outfile << "</" << levelName.c_str() << ">" << std::endl;

	outfile.close();

}
