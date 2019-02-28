#version 440

layout(std140, binding = 0) uniform Camera
{
	uniform mat4 uProj;
	uniform mat4 uView;
	uniform mat4 uProjInverse;
	uniform vec4 camPos;
	//uniform mat4 uViewInverse; // Camera Position
};
layout(location=0) uniform mat4 uModel;

layout(std140, binding = 1) uniform Time
{
	uniform float uTime;
};

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

out vec3 normal;
out vec2 texcoord;
out vec3 pos;

void main()
{
	vec3 newVert = in_vert;
	vec3 newNorm = in_normal;

	normal = mat3(uView) * mat3(uModel) * newNorm;
	texcoord = vec2(in_uv.x, 1.f - in_uv.y);

	vec4 viewSpace = uView * uModel * vec4(newVert, 1.0f);

	vec4 subStep = uProj * viewSpace;
	gl_Position = subStep;
	
	pos = viewSpace.xyz;
}