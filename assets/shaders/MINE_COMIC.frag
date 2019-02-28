#version 420

layout(location = 2) out vec3 outTexAlbedo;
layout(location = 3) out vec3 outTexSpecular;
layout(location = 0) out vec3 outLightDiffuse;
layout(location = 1) out vec3 outLightSpecular;
layout(location = 4) out vec3 unaffected;

layout (binding = 0) uniform sampler2D uTex;

in vec3 normal;
in vec2 texcoord;
in vec3 pos;

void main()
{
	outTexAlbedo = vec3(0);
	outTexSpecular = vec3(0);
	outLightDiffuse = vec3(0);
	outLightSpecular = vec3(0);
	unaffected = texture(uTex, texcoord).rgb;

	//outColor.a *= 0.5f;
}