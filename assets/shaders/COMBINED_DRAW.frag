#version 440

layout(std140, binding = 1) uniform Time
{
	uniform float uTime;
};

layout (binding = 0) uniform sampler2D uTexAlbedo;
layout (binding = 1) uniform sampler2D uTexEmissive;
layout (binding = 2) uniform sampler2D uTexRoughness;
layout (binding = 3) uniform sampler2D uTexMetallic;
layout (binding = 4) uniform sampler2D uLightAmbient;
layout (binding = 5) uniform sampler2D uLightDiffuse;
layout (binding = 6) uniform sampler2D uLightSpecular;

layout (binding = 30) uniform sampler2D uOverlay;
layout (binding = 31) uniform sampler2D uToonDiffuse;

uniform vec2 ASPECT;
uniform float texRot;

in vec2 texcoord;

layout (location = 0) out vec4 outColor;

void main()
{
	vec3 albedo		= texture(uTexAlbedo, texcoord).rgb;
	vec3 emissive	= texture(uTexEmissive, texcoord).rgb;
	vec3 roughness	= texture(uTexRoughness, texcoord).rgb;
	vec3 metallic	= texture(uTexMetallic, texcoord).rgb;
	vec3 ambient	= texture(uLightAmbient, texcoord).rgb;
	vec3 diffuse	= texture(uLightDiffuse, texcoord).rgb;
	vec3 specular	= texture(uLightSpecular, texcoord).rgb;
		
	mat2 tRot = mat2(cos(texRot), -sin(texRot), sin(texRot), cos(texRot));
	vec3 overlay = texture(uOverlay, texcoord * 0.02f * tRot * ASPECT.xy).rgb;

	vec3 difNorm;
	float tot = max(max(max(diffuse.r, diffuse.g), diffuse.b), 0);
	if (tot > 0.f)
		difNorm = diffuse / tot;

	
	float newDark = min(max(overlay.r - 1.f + 2.f * tot, 0.f), 1.f);

	//vec3 toonDiffuse = vec3(newDark) * difNorm;
	//toonDiffuse=  vec3(tot, 0.f, 1.f - tot);

	vec3 toonDiffuse = mix(texture(uToonDiffuse, vec2(tot, 0.f)), texture(uToonDiffuse, vec2(tot, 2.f)), newDark).rgb * difNorm;


	

	outColor.rgb = ambient;
	outColor.rgb += toonDiffuse * roughness * albedo;
	outColor.rgb += specular * metallic;
	outColor.rgb += emissive;
}