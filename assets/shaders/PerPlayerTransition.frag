#version 440

out vec4 outColor;

uniform sampler2D uTex;
uniform float blackout;

in vec2 texcoord;

void main()
{
	vec2 adjustable = texcoord;

	vec4 source = texture(uTex, adjustable);

	outColor = mix(source, vec4(0, 0, 0, 1), blackout);
}