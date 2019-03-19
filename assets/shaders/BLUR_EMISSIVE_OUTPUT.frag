#version 440

layout(location = 2)out vec4 outEmissive;
uniform sampler2D emissiveBlur;

in vec2 texcoord;

void main()
{
	outEmissive.rgb += texture(emissiveBlur, texcoord).rgb;
}