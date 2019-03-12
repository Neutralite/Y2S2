#version 440

layout(binding = 0) uniform sampler2D uTexAlbedo;
out vec4 outColor;

uniform vec3 colorShift;

in vec2 texcoord;

void main()
{
	vec2 adjustable = texcoord;

	vec4 source = texture(uTexAlbedo, adjustable) + vec4(colorShift, 0.f);
	if (source.a < 0.5f)
		discard;
	outColor = source;
}