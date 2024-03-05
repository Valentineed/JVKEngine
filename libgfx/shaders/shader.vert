#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 3, binding = 0) uniform UniformMVP {
    mat4 VP;
    mat4 model;
} ubo;

//layout(push_constant) uniform PushConsts { mat4 MVP; } pushConsts;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out int fragTexID;
layout(location = 3) out vec3 Normal;

void main() {
    gl_Position = ubo.VP * ubo.model * vec4(inPosition, 1.0);
    fragPos = vec3(ubo.model * vec4(inPosition, 1.0));
	Normal = mat3(transpose(inverse(ubo.model))) * inNormal;
    fragTexCoord = inTexCoord;
}