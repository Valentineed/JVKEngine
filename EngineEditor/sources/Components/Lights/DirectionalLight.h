#pragma once
#include "PointLight.h"

class DirectionalLight : public LightComponent
{
public:
	DirectionalLight(const Vector3& direction, const Vector3& ambientColor, const Vector3& diffuseColor, const Vector3& specularColor);
};
