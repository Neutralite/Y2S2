#version 440

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outEmissive;
layout (location = 3) out vec3 outRoughness;
layout (location = 4) out vec3 outMetallic;

layout(binding = 0) uniform sampler2D uTexAlbedo;
layout(binding = 1) uniform sampler2D uTexEmissive;
layout(binding = 2) uniform sampler2D uTexRoughness;
layout(binding = 3) uniform sampler2D uTexMetallic;
layout(binding = 4) uniform sampler2D uTexNormal;

uniform float uRoughness = 1.f;
uniform float uMetallic = 1.f;
uniform vec3 newColor;

in vec3 normal;
in vec2 texcoord;
in vec3 pos;
in vec3 tangent;
in vec3 bitangent;

void main()
{
	vec2 texOffset = texcoord;

	vec4 albedoColor = texture(uTexAlbedo, texOffset);
	if(albedoColor.a < 0.5)	discard;
	
	outColor = albedoColor.rgb * newColor;

	// Fix length after rasterizer interpolates
	// Get value of normal, tangent, bitangent
	vec3 norm = normalize(normal);
	vec3 tang = normalize(tangent);
	vec3 bitang = normalize(bitangent);

	//create a rotation matrix, tangentSpaceToViewSpace

	mat3 T_S_T_V_S = mat3(tang, bitang, norm);
	vec3 normalMap = texture(uTexNormal, texOffset).rgb * 2.f - 1.f;
	normalMap = T_S_T_V_S * normalMap;

	outNormal = normalMap * 0.5 + 0.5;
	//outNormal = normalize(normal * T_S_T_V_S) * 0.5 + 0.5;


	outEmissive = texture(uTexEmissive, texOffset).rgb * newColor;
	outRoughness = texture(uTexRoughness, texOffset).rgb * (uRoughness);
	outMetallic = texture(uTexMetallic, texOffset).rgb * (uMetallic);
}