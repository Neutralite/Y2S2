#version 440

out vec4 outColor;

layout(binding = 0)uniform sampler2D uTex;
layout(binding = 1)uniform sampler2D uDepth;

in vec2 texcoord;

void main()
{
	vec2 adjustable = texcoord;

	vec4 source = texture(uTex, adjustable);
	vec4 dCheck = texture(uDepth, adjustable);

	if (dCheck.r < 1.f)
		outColor += source;
}