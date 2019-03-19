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

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;

out vec2 texcoord;

void main()
{
	texcoord = (vec2(1.f - in_uv.x, 1.f - in_uv.y) * uTexDimensions.zw + uTexDimensions.xy) / uTextSize;
	texcoord = vec2(texcoord.x, 1.f - texcoord.y);

	mat4 tS = mat4(TotScale.x, 0, 0, 0, 0, TotScale.y, 0, 0, 0, 0, TotScale.z, 0, 0, 0, 0, 1.f);
	mat4 aS = mat4(aScale.x, 0, 0, 0, 0, aScale.y, 0, 0, 0, 0, aScale.z, 0, 0, 0, 0, 1.f);
	mat3 inv = inverse(mat3(uModel)) * inverse(mat3(uView));
	vec4 viewSpace = uView * uModel * mat4(inv)
		* (aS * (tS * vec4(in_vert, 1.0f) * vec4(xScale, 1.f, 1.f, 1.f) + vec4(uTextPos, 0.f)));

	vec4 subStep = uProj * viewSpace;
	gl_Position = subStep;
}