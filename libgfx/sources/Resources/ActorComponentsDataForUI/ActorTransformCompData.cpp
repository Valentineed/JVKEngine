#include "ActorTransformCompData.h"

libgfx_API LibMath::Vector3& ActorTransformCompData::GetPosition()
{
    return m_position;
}

void libgfx_API ActorTransformCompData::SetPosition(LibMath::Vector3 Position)
{
    m_position = Position;
}

libgfx_API LibMath::Vector3& ActorTransformCompData::GetRotation()
{
    return m_rotation;
}

void libgfx_API ActorTransformCompData::SetRotation(LibMath::Vector3 Rotation)
{
    m_rotation = Rotation;
}

libgfx_API LibMath::Vector3& ActorTransformCompData::GetScale()
{
    return m_scale;
}

void libgfx_API ActorTransformCompData::SetScale(LibMath::Vector3 Scale)
{
    m_scale = Scale;
}

void libgfx_API ActorTransformCompData::SetTransform(LibMath::Vector3 Position, LibMath::Vector3 Rotation, LibMath::Vector3 Scale)
{
    m_position = Position;

    m_rotation = Rotation;

    m_scale = Scale;
}
