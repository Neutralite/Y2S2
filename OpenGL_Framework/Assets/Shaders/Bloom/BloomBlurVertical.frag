#version 330

uniform sampler2D uTex;
uniform float uPixelSize; //1.0 / WH

in vec2 texcoord;
out vec3 outColor;

void main()
{
	outColor = vec3(0.0, 0.0, 0.0);
	outColor += texture(uTex, vec2(texcoord.x, texcoord.y - uPixelSize * 4.0)).rgb * 0.06;
	outColor += texture(uTex, vec2(texcoord.x, texcoord.y - uPixelSize * 3.0)).rgb * 0.09;
	outColor += texture(uTex, vec2(texcoord.x, texcoord.y - uPixelSize * 2.0)).rgb * 0.12;
	outColor += texture(uTex, vec2(texcoord.x, texcoord.y - uPixelSize * 1.0)).rgb * 0.15;
	outColor += texture(uTex, vec2(texcoord.x, texcoord.y - uPixelSize * 0.0)).rgb * 0.16;
	outColor += texture(uTex, vec2(texcoord.x, texcoord.y - uPixelSize * -1.0)).rgb * 0.15;
	outColor += texture(uTex, vec2(texcoord.x, texcoord.y - uPixelSize * -2.0)).rgb * 0.12;
	outColor += texture(uTex, vec2(texcoord.x, texcoord.y - uPixelSize * -3.0)).rgb * 0.09;
	outColor += texture(uTex, vec2(texcoord.x, texcoord.y - uPixelSize * -4.0)).rgb * 0.06;
}