@echo Should copy necessary directories near .exe
SET mypath=%~dp0

@echo %mypath%

call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
devenv %mypath%/../Build/JVKEngine.sln /build Shipping
robocopy %mypath%/../build/bin/Shipping D:\Dev\Isart\Build\Build/JVKEngine /E /mt /z
robocopy %mypath%/ D:\Dev\Isart\Build\Build/JVKEngine/resources /E /mt /z
robocopy %mypath%/../Assets/ D:\Dev\Isart\Build\Build/JVKEngine/Assets /E /mt /z

pause
