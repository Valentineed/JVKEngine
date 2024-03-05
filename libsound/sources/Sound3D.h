#pragma once
#include <fmod_common.h>
#include <string_view>
#include "libsound_Export.h"
#include "Sound.h"

struct Sound;

namespace LibMath {
	struct Vector3;
}

class SoundManager;

namespace FMOD {
	class Reverb3D;
}
class Sound3D
{
public:
	libsound_API Sound3D() = default;
	libsound_API ~Sound3D();
	libsound_API Sound3D(std::string_view path);
	libsound_API Sound3D(std::string_view path, const LibMath::Vector3& pos, float maxDist, float minDist);
	void libsound_API CreateReverb(const LibMath::Vector3& pos, float maxDist, float minDist);

	void libsound_API SetPos(const LibMath::Vector3& pos, const LibMath::Vector3& vel);

	void libsound_API SetAllSoundAttributes(const LibMath::Vector3& pos, float maxDist, float minDist);
	void libsound_API Active3DLinear() const;
	void libsound_API ActiveInverseMod() const;
	void libsound_API Active3DSquareLinear() const;

	[[nodiscard]] libsound_API Sound* GetSound();
private:
	Sound m_soundWrap;
	FMOD::Reverb3D* m_reverb = nullptr;
	SoundManager* m_manager = nullptr;
	float m_maxDist = 0;
	float m_minDist = 0;
	FMOD_VECTOR m_soundPos = { 0,0,0 };
	FMOD_VECTOR m_velocity = { 0,0,0 };
};