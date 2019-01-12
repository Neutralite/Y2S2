#version 420

out vec4 outColor;

layout (binding = 0) uniform sampler2D uTex;

in vec3 normal;
in vec2 texcoord;
in vec3 pos;

void main()
{
	outColor = texture(uTex, texcoord);

	outColor.a *= 0.5f;
}