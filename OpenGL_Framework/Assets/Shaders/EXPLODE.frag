#version 420

out vec4 outColor;

layout (binding = 0) uniform sampler2D uTex;
layout (binding = 1) uniform sampler2D uR;
layout (binding = 2) uniform sampler2D uG;

uniform float TimeRem;

in vec3 normal;
in vec2 texcoord;
in vec3 pos;

void main()
{
	float tRem = (1.f - TimeRem) * (1.f - TimeRem);
	float tLow = 1.f;
	float tHigh = 3.f;

	outColor = texture(uTex, texcoord);
	vec4 RMAP = texture(uR, texcoord);

	//vec3 outSub = outColor.rgb;

	outColor.rgb -= vec3(-RMAP.r * tHigh + tRem * (tHigh + 1.f), -RMAP.r * (tLow + tHigh) * 0.5f + tRem * (tHigh + 1.f), -RMAP.r * tLow + tRem * (tHigh + 1.f));

	if (outColor.r <= 0 && outColor.g <= 0 && outColor.b <= 0)
	{
		discard;
	}

	outColor.a = 1.f;
}