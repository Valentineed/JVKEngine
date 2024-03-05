#pragma once
#include <libgfx_Export.h>

#include "Vector/Vector3.h"

class ActorTransformCompData
{
public:
    ActorTransformCompData() = default;

    //Transform Component
    libgfx_API LibMath::Vector3& GetPosition();
    void libgfx_API SetPosition(LibMath::Vector3 Position);

    libgfx_API LibMath::Vector3& GetRotation();
    void libgfx_API SetRotation(LibMath::Vector3 Rotation);

    libgfx_API LibMath::Vector3& GetScale();
    void libgfx_API SetScale(LibMath::Vector3 Scale);

    void libgfx_API SetTransform(LibMath::Vector3 Position, LibMath::Vector3 Rotation, LibMath::Vector3 Scale);


private:

    //Transform component
    LibMath::Vector3 m_position = LibMath::Vector3::Zero;
    LibMath::Vector3 m_rotation = LibMath::Vector3::Zero;
    LibMath::Vector3 m_scale = LibMath::Vector3::Zero;


};
