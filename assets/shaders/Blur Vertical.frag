#version 440

out vec4 outColor;

uniform sampler2D uTex;
uniform float uSpread;
uniform vec2 ASPECT;

in vec2 texcoord;

void main()
{
	vec2 adjustable = texcoord;
	vec4 sumColor = vec4(0.f, 0.f, 0.f, 1.f);

	sumColor += texture(uTex, texcoord + vec2(0.f,  4.f) * uSpread * ASPECT) * 1.f;
	sumColor += texture(uTex, texcoord + vec2(0.f,  3.f) * uSpread * ASPECT) * 8.f;
	sumColor += texture(uTex, texcoord + vec2(0.f,  2.f) * uSpread * ASPECT) * 28.f;
	sumColor += texture(uTex, texcoord + vec2(0.f,  1.f) * uSpread * ASPECT) * 56.f;
	sumColor += texture(uTex, texcoord + vec2(0.f,  0.f) * uSpread * ASPECT) * 70.f;
	sumColor += texture(uTex, texcoord + vec2(0.f, -1.f) * uSpread * ASPECT) * 56.f;
	sumColor += texture(uTex, texcoord + vec2(0.f, -2.f) * uSpread * ASPECT) * 28.f;
	sumColor += texture(uTex, texcoord + vec2(0.f, -3.f) * uSpread * ASPECT) * 8.f;
	sumColor += texture(uTex, texcoord + vec2(0.f, -4.f) * uSpread * ASPECT) * 1.f;

	sumColor /= 256.f;

	outColor = sumColor;
}