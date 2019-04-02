#version 440

out vec4 outColor;

layout(binding = 0) uniform sampler2D uTex;
layout(binding = 1) uniform sampler3D LUT;

uniform float mixVal;

in vec2 texcoord;

void main()
{
	vec2 adjustable = texcoord;

	vec4 original = texture(uTex, adjustable);

	outColor = mix(original, texture(LUT, original.rgb), mixVal);
}