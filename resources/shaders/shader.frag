#version 450
#extension GL_ARB_separate_shader_objects : enable

//layout(binding = 0) uniform sampler Sampler;
//layout(binding = 1) uniform texture2D Textures[];

#define MAXLIGHTS 10

struct Light{
    vec3 lightDir;
    float cutOff;
    float outerCutOff;
	
    vec4 lightPos;
	
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
	
	bool bIsPoint;
	
    float kConstant;
    float kLinear;
    float kQuadratic;
	
	vec4 off;
};

layout(set = 0, binding = 0) uniform LightBufferObject{
	Light light[MAXLIGHTS]; 
};

layout(set = 1, binding = 0) uniform sampler2D texSampler;

layout(set = 2, binding = 0) uniform UniformViewPos {
	vec3 viewPos;
};

//layout(location = 0) in vec3 fragColor;
layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in int fragTexID;
layout(location = 3) in vec3 Normal;

layout(location = 0) out vec4 outColor;

//Substitute for material values
float ambientStrength = 0.3;
float specularStrength = 0.5;
float diffuseStrength = 0.2;
float shininess = 32;

void main() {
    //vec3 ambient = ambientStrength * lbo.lightColor;

    //vec3 result = ambient * fragColor;
    //outColor = texture(texSampler, fragTexCoord) * vec4(result, 1.0f);
    ////outColor = texture(texSampler[nonuniformEXT(fragTexID)], fragTexCoord) * vec4(fragColor, 1.0f);
		
	
	vec3 result = vec3(0);
	for(int i = 0; i < MAXLIGHTS; i++)
	{
		if(light[i].lightPos != vec4(0.0))
		{
			if (light[i].lightPos.w  ==  0.0 )
			{			
				//DIRECTIONAL
				/*Ambient*/
				vec3 ambient = vec3(light[i].ambient * ambientStrength);

				/*Diffuse*/
				vec3 norm = normalize(Normal);
				vec3 lightDir = normalize(-light[i].lightPos.xyz);
				float diff = max(dot(norm, lightDir), 0.0);
				vec3 diffuse = vec3((diff * diffuseStrength) * light[i].diffuse);

				/*Specular*/
				vec3 viewDir = normalize(viewPos - fragPos);
				vec3 reflectDir = reflect(-lightDir, norm);  
				float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
				vec3 specular = vec3((specularStrength * spec) * light[i].specular);
				result += (ambient + diffuse + specular);
				outColor = texture(texSampler, fragTexCoord) * vec4(result, 1.0f);
			}
			else  
			{ 
				if(light[i].bIsPoint == true)
				{
					//POINT
					/*Ambient*/
					vec3 ambient = vec3(light[i].ambient * ambientStrength);

					/*Attenuation*/
					float distance    = length(light[i].lightPos.xyz - fragPos);
					float attenuation = 1.0 / (light[i].kConstant + light[i].kLinear * distance + light[i].kQuadratic * (distance * distance)); 

					/*Diffuse*/
					vec3 norm = normalize(Normal);
					vec3 lightDir = normalize(light[i].lightPos.xyz - fragPos);
					float diff = max(dot(norm, lightDir), 0.0);
					vec3 diffuse = vec3((diff * diffuseStrength) * light[i].diffuse);

					/*Specular*/
					vec3 viewDir = normalize(viewPos - fragPos);
					vec3 reflectDir = reflect(-lightDir, norm);  
					float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
					vec3 specular = vec3((specularStrength * spec) * light[i].specular);
					
					result += (ambient + diffuse + specular) * attenuation;
				}
				else{
					//SPOTLIGHT
					/*Ambient*/
					vec3 ambient = vec3(light[i].ambient * ambientStrength);

					/*Attenuation*/
					float distance    = length(light[i].lightPos.xyz - fragPos);
					float attenuation = 1.0 / (light[i].kConstant + light[i].kLinear * distance + light[i].kQuadratic * (distance * distance)); 

					/*Diffuse*/
					vec3 norm = normalize(Normal);
					vec3 lightDir = normalize(light[i].lightPos.xyz - fragPos);
					float diff = max(dot(norm, lightDir), 0.0);
					vec3 diffuse = vec3((diff * diffuseStrength) * light[i].diffuse);

					/*Specular*/
					vec3 viewDir = normalize(viewPos - fragPos);
					vec3 reflectDir = reflect(-lightDir, norm);  
					float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
					vec3 specular = vec3((specularStrength * spec) * light[i].specular);

					/*SpotLight Test*/
					float theta = dot(lightDir, normalize(-light[i].lightDir));
					float epsilon = light[i].cutOff - light[i].outerCutOff;
					float intensity = clamp((theta - light[i].outerCutOff)/epsilon, 0.0, 1.0);

					
					//FragColor = vec4(0.f, 0.f, 1.f, 1.0f);
					result += (ambient + (diffuse * intensity) + (specular * intensity)) * attenuation;
				}
			}
		}
		outColor = texture(texSampler, fragTexCoord) * vec4(result, 1.0f);
	}
}