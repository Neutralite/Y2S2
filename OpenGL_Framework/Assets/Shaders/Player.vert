#version 330

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

//uniform int Vsize;
//uniform vec4 bulges[100];

//uniform float WW;
//uniform float WH;

uniform vec3 dirOfMove;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

layout(location = 3) in vec3 in_vert2;
layout(location = 4) in vec2 in_uv2;
layout(location = 5) in vec3 in_normal2;

layout(location = 6) in vec3 in_vert3;
layout(location = 7) in vec2 in_uv3;
layout(location = 8) in vec3 in_normal3;

out vec3 normal;
out vec2 texcoord;
out vec3 pos;
out mat4 MVmat;

void main()
{
	float lower = 0.1f;
	vec3 daVert = in_vert + (in_vert2 - in_vert) * (dirOfMove.z * lower) + (in_vert3 - in_vert) * (dirOfMove.x * lower);

	normal = mat3(uView) * mat3(uModel) * in_normal;
	texcoord = vec2(in_uv.x, 1.f - in_uv.y);

	MVmat = uView;
	vec4 viewSpace = uView * uModel * vec4(daVert, 1.0f);

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