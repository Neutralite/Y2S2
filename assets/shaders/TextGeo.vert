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
uniform float xScale;
uniform vec3 uTextPos;
uniform vec4 uTexDimensions;
uniform vec3 TotScale;
uniform vec2 uTextSize;
uniform vec3 aScale;

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
	vec3 newVert = in_vert;
	vec3 newNorm = in_normal;

	normal = newNorm;
	tangent = in_tangent;
	bitangent = in_bitangent;
	texcoord = (vec2(1.f - in_uv.x, 1.f - in_uv.y) * uTexDimensions.zw + uTexDimensions.xy) / uTextSize;
	texcoord = vec2(texcoord.x, 1.f - texcoord.y);

	//vec4 viewSpace = vec4(inverse(mat3(uModel)) * inverse(mat3(uView)) * vec3(uView * uModel * 
	//	(mat4(TotScale.x, 0, 0, 0, 0, TotScale.y, 0, 0, 0, 0, TotScale.z, 0, 0, 0, 0, 1.f) * (vec4(newVert, 1.0f) * vec4(xScale, 1.f, 1.f, 1.f) + vec4(uTextPos, 0.f)))), 1.f);

	mat4 tS = mat4(TotScale.x, 0, 0, 0, 0, TotScale.y, 0, 0, 0, 0, TotScale.z, 0, 0, 0, 0, 1.f);
	mat4 aS = mat4(aScale.x, 0, 0, 0, 0, aScale.y, 0, 0, 0, 0, aScale.z, 0, 0, 0, 0, 1.f);
	mat3 inv = inverse(mat3(uModel)) * inverse(mat3(uView));
	vec4 viewSpace = uView * uModel * mat4(inv)
		* (aS * (tS * vec4(newVert, 1.0f) * vec4(xScale, 1.f, 1.f, 1.f) + vec4(uTextPos, 0.f)));
	//viewSpace = mat4(inv) * viewSpace;

	//vec4 viewSpace = vec4(vec3(uView * uModel * 
	//	(mat4(TotScale.x, 0, 0, 0, 0, TotScale.y, 0, 0, 0, 0, TotScale.z, 0, 0, 0, 0, 1.f) * (vec4(newVert, 1.0f) * vec4(xScale, 1.f, 1.f, 1.f) + vec4(uTextPos, 0.f)))), 1.f);

	vec4 subStep = uProj * viewSpace;
	gl_Position = subStep;
	
	pos = viewSpace.xyz;
}