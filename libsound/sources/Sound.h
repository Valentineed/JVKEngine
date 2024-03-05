#pragma once
#include "../../EngineEditor/sources/Resources/ResourcesManager.h"

namespace FMOD {
	class Channel;
	class Sound;
}

struct Sound2D : Resources::ResourcesManager
{
	FMOD::Sound* m_sound;
	FMOD::Channel* m_chanel;
};
struct Sound
{
	FMOD::Sound* m_sound;
	FMOD::Channel* m_chanel;
};
