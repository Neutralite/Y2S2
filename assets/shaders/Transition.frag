#version 440

out vec4 outColor;

layout(binding = 0) uniform sampler2D uTex;
layout(binding = 1) uniform sampler2D uTransition;
layout(binding = 2) uniform sampler2D uFade;
layout(binding = 3) uniform sampler2D uTransOver;
uniform float tTime = 0.f;

in vec2 texcoord;

void main()
{
	vec2 adjustable = vec2(texcoord.x, 1.f - texcoord.y);

	float transitionPiece = texture(uTransition, adjustable).r * 0.999f;
	float _FADE = texture(uFade, adjustable).r;
	outColor = mix(texture(uTex, texcoord), vec4(1), clamp(_FADE + 2.f * tTime - 1.f, 0.f, 1.f));

	if (transitionPiece - tTime < 0)
		outColor.rgb = texture(uTransOver, adjustable).rgb;
}