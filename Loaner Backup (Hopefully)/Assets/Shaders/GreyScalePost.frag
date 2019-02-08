#version 330

out vec4 outColor;

uniform sampler2D uTex;
uniform float totTime;
uniform float WW;
uniform float WH;

in vec2 texcoord;

void main()
{
	vec2 adjustable = texcoord;

	float T = WW / WH;

	adjustable.x *= T;
	vec2 sub = adjustable - vec2(T * 0.5, 0.5);
	
	adjustable += normalize(sub) * sin(length(sub) * 2 * 3.14159 * 4 - totTime * 3.14159 * 4) / 100;

	adjustable.x /= T;

	vec4 source = texture(uTex, adjustable);

	float luminance = 0.2989 * source.r + 0.587 * source.g + 0.114 * source.b;

	outColor = source;
}