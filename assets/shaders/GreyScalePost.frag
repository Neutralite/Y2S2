#version 440

out vec4 outColor;

uniform sampler2D uTex;

in vec2 texcoord;

void main()
{
	vec2 adjustable = texcoord;

	vec4 source = texture(uTex, adjustable);

	outColor = source;
}