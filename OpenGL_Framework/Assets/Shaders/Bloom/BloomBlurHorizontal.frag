#version 330

uniform sampler2D uTex;
uniform float uPixelSize; //1.0 / WW

in vec2 texcoord;
out vec3 outColor;

void main()
{
	outColor = vec3(0.0, 0.0, 0.0);
	outColor += texture(uTex, vec2(texcoord.x - uPixelSize * 4.0, texcoord.y)).rgb * 0.06;
	outColor += texture(uTex, vec2(texcoord.x - uPixelSize * 3.0, texcoord.y)).rgb * 0.09;
	outColor += texture(uTex, vec2(texcoord.x - uPixelSize * 2.0, texcoord.y)).rgb * 0.12;
	outColor += texture(uTex, vec2(texcoord.x - uPixelSize * 1.0, texcoord.y)).rgb * 0.15;
	outColor += texture(uTex, vec2(texcoord.x - uPixelSize * 0.0, texcoord.y)).rgb * 0.16;
	outColor += texture(uTex, vec2(texcoord.x - uPixelSize * -1.0, texcoord.y)).rgb * 0.15;
	outColor += texture(uTex, vec2(texcoord.x - uPixelSize * -2.0, texcoord.y)).rgb * 0.12;
	outColor += texture(uTex, vec2(texcoord.x - uPixelSize * -3.0, texcoord.y)).rgb * 0.09;
	outColor += texture(uTex, vec2(texcoord.x - uPixelSize * -4.0, texcoord.y)).rgb * 0.06;
}