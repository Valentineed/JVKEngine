#pragma once
#include <sstream>

class TiXmlElement;
class TiXmlNode;
class Actor;
class Scene;
namespace LibMath
{
	struct Vector3;
	struct Vector4;
	struct Matrix4;
}

namespace XML
{
	class Serializer
	{
	public:
		~Serializer() = default;
		Serializer(Serializer const&) = delete;
		Serializer& operator=(Serializer const&) = delete;
		
		static Serializer& GetInstance();

		void ExportScene(Scene* scene);
		Scene LoadScene(const char* filename);

		bool m_IfOnDefaultMap = true;
		std::string m_currentScenePath;
		bool m_bCustom;
	private:
		Serializer() = default;

		float TextTofloat(const char* text);

		std::stringstream MatrixToText(LibMath::Matrix4& mat);
		LibMath::Matrix4 TextToMatrix(const char* text);

		std::stringstream Vector3ToText(const LibMath::Vector3& vec);
		LibMath::Vector3 TextToVec3(const char* text);

		std::stringstream Vector4ToText(const LibMath::Vector4& vec);
		LibMath::Vector4 TextToVec4(const char* text);

		Actor* GetActorType(const char* text, std::pair<std::string, std::string> paths = {"",""});
		void SaveRecursive(TiXmlElement* parent, Actor& parentActor);
		void LoadRecursive(TiXmlNode* pParent, Actor& parentActor, Scene& sceneToFill);

		
	};
}
