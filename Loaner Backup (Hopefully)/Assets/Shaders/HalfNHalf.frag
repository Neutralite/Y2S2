#version 330

out vec4 outColor;

uniform sampler2D uTex;

in vec3 normal;
in vec2 texcoord;
in vec3 pos;

void main()
{
	outColor = texture(uTex, texcoord);
}