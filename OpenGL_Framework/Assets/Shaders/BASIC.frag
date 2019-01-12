#version 330

out vec4 outColor;

uniform sampler2D uTex;
uniform bool HIGHLIGHT;

in vec3 normal;
in vec2 texcoord;
in vec3 pos;
in float LocX;
in float LocY;

void main()
{
	outColor = texture(uTex, texcoord);
	if (HIGHLIGHT)
	{
		outColor.rgb *= 0.9f;
		outColor.g = 1;
	}
	outColor.a *= 0.5f;
}