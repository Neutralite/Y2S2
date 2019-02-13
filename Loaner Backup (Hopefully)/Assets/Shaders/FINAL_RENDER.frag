#version 420

uniform vec3 uSceneAmbient;

layout(binding = 0)uniform sampler2D LD;
layout(binding = 1)uniform sampler2D LS;
layout(binding = 2)uniform sampler2D TA;
layout(binding = 3)uniform sampler2D TS;

uniform vec3 ASPECT;
layout (binding = 20) uniform sampler2D overlay;

in vec2 texcoord;

out vec4 outColor;

void main()
{
	vec2 texOffset = texcoord;

	vec4 albedoColor = texture(TA, texOffset);
	outColor.rgb = texture(TA, texcoord).rgb * uSceneAmbient; 
	outColor.a = texture(TA, texcoord).a;

	vec2 newOffset = texOffset * ASPECT.xy / 300.f;
	vec4 newSpec = texture(overlay, newOffset) - vec4(1.f, 1.f, 1.f, 1.f) + 2.f * texture(LS, texcoord);
	vec4 newDif = texture(overlay, newOffset) - vec4(1.f, 1.f, 1.f, 1.f) + 2.f * texture(LD, texcoord);

	newSpec.x = max(min(newSpec.x, 1.f), 0.f);
	newSpec.y = max(min(newSpec.y, 1.f), 0.f);
	newSpec.z = max(min(newSpec.z, 1.f), 0.f);
	newSpec.w = max(min(newSpec.w, 1.f), 0.f);

	newDif.x = max(min(newDif.x, 1.f), 0.f);
	newDif.y = max(min(newDif.y, 1.f), 0.f);
	newDif.z = max(min(newDif.z, 1.f), 0.f);
	newDif.w = max(min(newDif.w, 1.f), 0.f);

	outColor.rgb += texture(TA, texcoord).rgb * newDif.rgb;
	outColor.rgb += texture(TS, texcoord).rgb * newSpec.rgb;

	//outColor.rgb += texture(overlay, newOffset).rgb;
	//outColor.rgb += texture(uTexEmissive, texOffset).rgb;
}