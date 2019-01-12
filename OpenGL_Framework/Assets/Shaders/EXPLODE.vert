#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

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

void main()
{
	normal = mat3(uView) * mat3(uModel) * in_normal;
	texcoord = vec2(in_uv.x, 1.f - in_uv.y);

	vec4 viewSpace = uView * uModel * vec4(in_vert, 1.0f);

	vec4 subStep = uProj * viewSpace;
	gl_Position = subStep;
	
	pos = viewSpace.xyz;
}