#version 330

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

uniform float BEZTIME;
uniform vec3 BP1;
uniform vec3 BP2;
uniform float DT;
uniform float YEET;

//uniform int Vsize;
//uniform vec4 bulges[100];

//uniform float WW;
//uniform float WH;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

out vec3 normal;
out vec2 texcoord;
out vec3 pos;
out mat4 MVmat;

void main()
{
	vec3 newVert = in_vert;
	//newVert.y += (uModel * vec4(0, 0, 0, 1)).y;
	vec3 newNorm = in_normal;
	if (YEET > 0.f)
	{
		float yeeters = 1.f + 0.02f * sin(1.5707f * sin(3.f * DT + YEET));
		newVert = vec3(in_vert.x / yeeters, in_vert.y * yeeters * yeeters, in_vert.z / yeeters);
		vec3 Norm2 = vec3(in_normal.x * yeeters, in_normal.y / yeeters / yeeters, in_normal.z * yeeters);
		newNorm = normalize(Norm2);
	}
	//newVert.y -= (uModel * vec4(0, 0, 0, 1)).y;

	vec4 spacing = uModel * vec4(newVert, 1.0f);
	vec4 jolting = uModel * vec4(0, 0, 0, 1);

	vec4 substep1 = mix(spacing, spacing + vec4(BP1, 0) * 0.5f * (in_vert.y + jolting.y), BEZTIME);
	vec4 substep2 = mix(spacing + vec4(BP1, 0) * 0.5f * (in_vert.y + jolting.y), spacing + vec4(BP2, 0) * 0.5f * (in_vert.y + jolting.y), BEZTIME);
	vec4 substep3 = mix(spacing + vec4(BP2, 0) * 0.5f * (in_vert.y + jolting.y), spacing, BEZTIME);
	   
	vec4 substep4 = mix(substep1, substep2, BEZTIME);
	vec4 substep5 = mix(substep2, substep3, BEZTIME);
	   
	vec4 substep6 = mix(substep4, substep5, BEZTIME);

	normal = mat3(uView) * mat3(uModel) * newNorm;
	texcoord = vec2(in_uv.x, 1.f - in_uv.y);

	MVmat = uView;
	vec4 viewSpace = uView * substep6;

	vec4 subStep = uProj * viewSpace;
	gl_Position = subStep;
	//for (int i = 0; i < Vsize && i < 100; i++)
	//{
	//	float time = bulges[i].z - bulges[i].w;
	//	vec2 dis = vec2(WW / WH * (subStep.x - bulges[i].x), subStep.y - bulges[i].y);
	//	float T = (time + 0.6f) * (time + 0.6f) * length(dis - 50.0f * normalize(dis) * time);
	//	vec2 inBetween = normalize(dis) * 2.0f * T / ((T * T + 1.0f) * ( T * T + 1.0f) * (time + 0.6f) * (time + 0.6f));
	//	gl_Position += vec4(inBetween.x, inBetween.y, 0, 0);
	//}
	
	pos = viewSpace.xyz;
}