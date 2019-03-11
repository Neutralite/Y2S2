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

layout(std140, binding = 4) uniform Resolution
{
	uniform vec2 uResolution;
	uniform vec2 uPixelSize;
};

#define uLightConstantAttenuation uLightAttenuation.x
#define uLightLinearAttenuation uLightAttenuation.y
#define uLightQuadAttenuation uLightAttenuation.z
#define uLightMaxAngle uLightColor.w

//layout(binding = 0)uniform sampler2D uAlbedo;
//layout(binding = 1)uniform sampler2D uSpecular;
//layout(binding = 2)uniform sampler2D uNormals;
//layout(binding = 3)uniform sampler2D uDepth;
layout(binding = 31)uniform sampler2D uRamp;
//layout(binding = 4)uniform sampler2D uBase;

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
	vec2 Texcoord = uPixelSize * gl_FragCoord.xy;
	float TriCheck = doubleCheck;

	//outColor.rgb = texture(uBase,Texcoord).rgb;
	mat4 MV = uView * uModel;

	float specularExp = 16.f;
	vec2 texOffset = Texcoord;

	float depth = texture(uDepth, Texcoord).r;
	vec3 texSpec = texture(uMetallic, Texcoord).rgb;
	vec3 texAlb = texture(uAlbedo, Texcoord).rgb;

	vec4 Pos = uProjInverse * (vec4(Texcoord,depth,1.f) * 2.f - 1.f);
	Pos /= Pos.w;
	vec3 pos = vec3(Pos);
	vec3 N = normalize(texture(uNormals, Texcoord).rgb * 2.f - 1.f);
	vec3 Lvec = -(vec3(MV * vec4(uLightPosition, 1)) - pos);
	float dist = length(Lvec);
	float ATT = 1.0f / (uLightConstantAttenuation + uLightLinearAttenuation * dist + uLightQuadAttenuation * dist * dist);
	
	vec3 Ldir = Lvec / dist;

	float NdotL = -dot(Ldir, N);

	vec3 D = normalize((mat3(MV) * normalize(uLightDirection)).xyz);
	float isInside = -dot(Ldir, D);

	//NdotL = 0.5f * NdotL + 0.5f;

	if (NdotL > 0 && isInside > cos(uLightMaxAngle))
	{
		float NdotHV = max(dot(N, normalize(Ldir + normalize(-pos))), 0.0f);

		//outColor.rgb += NdotL * texAlb * ATT * uLightColor.rgb;
		//outColor.rgb += pow(NdotHV, specularExp) * ATT * texSpec;

		diffuseLight.rgb += NdotL * ATT * uLightColor.rgb * intensity;
		specularLight.rgb += pow(NdotHV, specularExp) * ATT * intensity;

		//outColor.rgb += texture(uRamp, vec2(NdotL, 0)).rgb * texAlb * ATT * uLightColor.rgb;
		//outColor.rgb += pow(texture(uRamp, vec2(NdotHV, 0)).r, specularExp) * ATT * texSpec;
	}
	//outColor.rgb = vec3(mod(pos.x, 1), mod(pos.y, 1), mod(pos.z, 1));
}