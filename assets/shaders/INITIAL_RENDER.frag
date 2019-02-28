#version 440

layout(location = 0) out vec3 outTexAlbedo;
layout(location = 1) out vec3 outTexSpecular;
layout(location = 2) out vec3 outTexNormal;

uniform sampler2D uTex;

in vec3 normal;
in vec2 texcoord;
in vec3 pos;

void main()
{
	vec3 N = normalize(normal);
	outTexSpecular = vec3(1);

	outTexNormal = (N + 1) * 0.5f;
	
	vec4 texCol = texture(uTex, texcoord);
	if (texCol.a < 0.5)
		discard;
	outTexAlbedo.rgb = texCol.rgb;
}