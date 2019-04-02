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
#define uLightMaxAngle uLightAttenuation.w

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
layout (binding = 10) uniform sampler2D uSpread;

layout (binding = 30) uniform sampler2D uTexShadowDepth;

uniform int DO_SHADOWS;
uniform int COLOR_SPLIT_SHADOWS;
uniform float splitAmount;
uniform mat4 uShadowView;
uniform mat4 uShadowCamView;
uniform mat4 uShadowCamProj;

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

	vec3 D = normalize((mat3(MV) * normalize(uLightDirection)).xyz);

	float NdotL = -dot(D, N);

	//NdotL = 0.5f * NdotL + 0.5f;
	
	float shadowMult = 1.f;

	vec3 letThrough = vec3(1.f);
	if (DO_SHADOWS == 1 && COLOR_SPLIT_SHADOWS == 1)
	{
		vec4 shadowCoord = uShadowView * Pos;
		float shadowDepth = texture(uTexShadowDepth, (shadowCoord).xy).r;

		float difference = max(shadowCoord.z + clamp(tan(acos(dot(N, D))) * 0.001f, -0.1f, 0.1f) - shadowDepth, 0);
		vec4 realPos = inverse(uShadowCamProj) * (2.f * vec4(shadowCoord.xy, shadowDepth, 1.f) - 1);
		realPos /= realPos.w;
		realPos = vec4(0.5f) + 0.5f * uProj * uView * inverse(uShadowCamView) * realPos;

		vec3 colSplit = 2.f * texture(uSpread, realPos.xy).rgb - 1.f;
		vec3 newNorm = mat3(uShadowCamView) * inverse(mat3(uView)) * N;

		vec2 tCoorShiftR = vec2(newNorm.y, -newNorm.x) * splitAmount * colSplit.r * difference + shadowCoord.xy;
		vec2 tCoorShiftG = vec2(newNorm.y, -newNorm.x) * splitAmount * colSplit.g * difference + shadowCoord.xy;
		vec2 tCoorShiftB = vec2(newNorm.y, -newNorm.x) * splitAmount * colSplit.b * difference + shadowCoord.xy;
		
		if (texture(uTexShadowDepth, tCoorShiftR).r < shadowCoord.z + clamp(tan(acos(dot(N, D))) * 0.001f, -0.1f, 0.1f))
		{
			letThrough.r = 0.f;
		}

		if (texture(uTexShadowDepth, tCoorShiftG).r < shadowCoord.z + clamp(tan(acos(dot(N, D))) * 0.001f, -0.1f, 0.1f))
		{
			letThrough.g = 0.f;
		}

		if (texture(uTexShadowDepth, tCoorShiftB).r < shadowCoord.z + clamp(tan(acos(dot(N, D))) * 0.001f, -0.1f, 0.1f))
		{
			letThrough.b = 0.f;
		}
	}
	else if (DO_SHADOWS == 1)
	{
		vec4 shadowCoord = uShadowView * Pos;
		float shadowDepth = texture(uTexShadowDepth, (shadowCoord).xy).r;
		if (shadowDepth < shadowCoord.z + clamp(tan(acos(dot(N, D))) * 0.001f, -0.1f, 0.1f))
		{
			shadowMult = 0.f;
		}
	}

	if (NdotL > 0)
	{
		float NdotHV = max(dot(N, normalize(-D + normalize(-pos))), 0.0f);
		
		diffuseLight.rgb += NdotL * uLightColor.rgb * intensity * letThrough * shadowMult;
		specularLight.rgb += pow(NdotHV, specularExp) * intensity * letThrough * shadowMult;
	}
}