#version 440

layout(std140, binding = 0) uniform Camera
{
	uniform mat4 uProj;
	uniform mat4 uView;
	uniform mat4 uProjInverse;
	uniform vec4 camPos;
	//uniform mat4 uViewInverse; // Camera Position
};
layout(std140, binding = 1) uniform Time
{
	uniform float uTime;
};
uniform mat4 uModel;

layout(std140, binding = 3) uniform Light
{
	uniform vec4 uLightColor;
	uniform vec3 uLightPosition;
	uniform vec3 uLightDirection;
	uniform vec4 uLightAttenuation; 
	uniform float intensity;
};

#define uLightConstantAttenuation uLightAttenuation.x
#define uLightLinearAttenuation uLightAttenuation.y
#define uLightQuadAttenuation uLightAttenuation.z
#define uLightMaxAngle uLightAttenuation.w

//layout(binding = 0)uniform sampler2D uAlbedo;
//layout(binding = 1)uniform sampler2D uSpecular;
//layout(binding = 2)uniform sampler2D uNormals;
//layout(binding = 3)uniform sampler2D uDepth;

layout (binding = 0) uniform sampler2D uAlbedo;
layout (binding = 1) uniform sampler2D uNormals;
layout (binding = 2) uniform sampler2D uEmissive;
layout (binding = 3) uniform sampler2D uRoughness;
layout (binding = 4) uniform sampler2D uMetallic;
layout (binding = 5) uniform sampler2D uDepth;

uniform float doubleCheck;

in vec2 texcoord;

layout (location = 0) out vec4 ambientLight;
layout (location = 1) out vec4 diffuseLight;
layout (location = 2) out vec4 specularLight;

void main()
{
	float TriCheck = doubleCheck;

	mat4 MV = uView * uModel;

	float specularExp = 16.f;
	vec2 texOffset = texcoord;

	float depth = texture(uDepth, texcoord).r;
	vec3 texSpec = texture(uMetallic, texcoord).rgb;
	vec3 texAlb = texture(uAlbedo, texcoord).rgb;

	vec4 Pos = uProjInverse * (vec4(texcoord,depth,1.f) * 2.f - 1.f);
	Pos /= Pos.w;
	vec3 pos = vec3(Pos);
	vec3 N = normalize(texture(uNormals, texcoord).rgb * 2.f - 1.f);
	vec3 Lvec = -(vec3(MV * vec4(uLightPosition, 1)) - pos);
	float dist = length(Lvec);
	float ATT = 1.0f / (uLightConstantAttenuation + uLightLinearAttenuation * dist + uLightQuadAttenuation * dist * dist);

	//vec3 Ldir = Lvec / dist;
	//float NdotL = -dot(Ldir, N);

	//outColor.rgb = vec3(NdotL * 2.f - 1.f);

	//outColor.rgb = 0.1f * texAlb;

	ambientLight.rgb = 0.1f * texAlb;
	diffuseLight.rgb = vec3(0.f);
	specularLight.rgb = vec3(0.f);
	//outColor.rgb = vec3(lightType);
}