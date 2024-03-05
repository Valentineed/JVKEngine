#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(const Vector3& direction, const Vector3& ambientColor, const Vector3& diffuseColor, const Vector3& specularColor)
{
	m_lightPos = { direction.x, direction.y, direction.z, 0.0f };
	m_ambientColor = ambientColor;
	m_diffuseColor = diffuseColor;
	m_specularColor = specularColor;
	type = Type::DIRECTIONAL;
}
