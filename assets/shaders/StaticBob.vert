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
uniform float bob;
uniform vec3 sway;

layout(std140, binding = 1) uniform Time
{
	uniform float uTime;
};

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec3 in_bitangent;

out vec3 normal;
out vec2 texcoord;
out vec3 pos;
out vec3 tangent;
out vec3 bitangent;

void main()
{
	float pop = 1.f + 0.02f * sin(1.5707f * sin(3.f * uTime + bob));
	vec3 newVert = vec3(in_vert.x / pop, in_vert.y * pop * pop, in_vert.z / pop);
	vec3 newNorm = vec3(in_normal.x * pop, in_normal.y / pop / pop, in_normal.z * pop);
	newNorm = normalize(newNorm);

	normal = mat3(uView) * mat3(uModel) * newNorm;
	tangent = mat3(uView) * mat3(uModel) * in_tangent;
	bitangent = mat3(uView) * mat3(uModel) * in_bitangent;
	texcoord = vec2(in_uv.x, 1.f - in_uv.y);

	vec4 viewSpace = uView * (uModel * vec4(newVert, 1.0f) + vec4(sway * in_vert.y, 0.f));

	vec4 subStep = uProj * viewSpace;
	gl_Position = subStep;
	
	pos = viewSpace.xyz;
}