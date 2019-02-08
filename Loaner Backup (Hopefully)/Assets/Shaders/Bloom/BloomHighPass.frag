#version 330

uniform sampler2D uTex; //source image
uniform float uThreshold;

in vec2 texcoord;

out vec3 outColor;

void main()
{
	vec3 color = texture(uTex, texcoord).rgb;

	float luminance = (color.r + color.g + color.b) / 3.0;

	if (luminance > uThreshold)
	{
		outColor = color;
	}
	else
	{
		outColor = vec3(0,0,0);
	}
}