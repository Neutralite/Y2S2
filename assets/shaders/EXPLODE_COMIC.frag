#version 420

layout(location = 2) out vec3 outTexAlbedo;
layout(location = 3) out vec3 outTexSpecular;
layout(location = 0) out vec3 outLightDiffuse;
layout(location = 1) out vec3 outLightSpecular;
layout(location = 4) out vec3 unaffected;

layout (binding = 0) uniform sampler2D uTex;
layout (binding = 1) uniform sampler2D uR;
layout (binding = 2) uniform sampler2D uG;

uniform float TimeRem;

in vec3 normal;
in vec2 texcoord;
in vec3 pos;

void main()
{
	outTexAlbedo = vec3(0);
	outTexSpecular = vec3(0);
	outLightDiffuse = vec3(0);
	outLightSpecular = vec3(0);

	float tRem = (1.f - TimeRem) * (1.f - TimeRem);
	float tLow = 1.f;
	float tHigh = 3.f;

	unaffected = texture(uTex, texcoord).rgb;
	vec4 RMAP = texture(uR, texcoord);

	//vec3 outSub = outColor.rgb;

	unaffected.rgb -= vec3(-RMAP.r * tHigh + tRem * (tHigh + 1.f), -RMAP.r * (tLow + tHigh) * 0.5f + tRem * (tHigh + 1.f), -RMAP.r * tLow + tRem * (tHigh + 1.f));

	if (unaffected.r <= 0 && unaffected.g <= 0 && unaffected.b <= 0)
	{
		discard;
	}

	//outColor.a = 1.f;
}