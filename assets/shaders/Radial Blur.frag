#version 440

out vec4 outColor;

uniform sampler2D uTex;
uniform float uAngle;
uniform vec2 uCenter;
uniform float ASPECT_RAT;

in vec2 texcoord;

void main()
{
	vec2 adjustable = texcoord;
	mat2 offput;
	float segmentSize = uAngle / 6.f;
	float dividedAngle = -uAngle * 0.5f;
	vec4 sumColor = vec4(0.f, 0.f, 0.f, 1.f);

	//newCenter = uCenter * vec2(1.f / ASPECT_RAT, 1.f);
	vec2 AR = vec2(ASPECT_RAT, 1.f);
	


	offput = mat2(	cos(dividedAngle), -sin(dividedAngle), 
					sin(dividedAngle), cos(dividedAngle));
	adjustable = offput * (texcoord - uCenter) + uCenter;
	adjustable = (adjustable - texcoord) * AR + texcoord;
	sumColor.rgb += texture(uTex, adjustable).rgb			* 1.f;
	dividedAngle += segmentSize;

	offput = mat2(	cos(dividedAngle), -sin(dividedAngle), 
					sin(dividedAngle), cos(dividedAngle));
	adjustable = offput * (texcoord - uCenter) + uCenter;
	adjustable = (adjustable - texcoord) * AR + texcoord;
	sumColor.rgb += texture(uTex, adjustable).rgb			* 6.f;
	dividedAngle += segmentSize;

	offput = mat2(	cos(dividedAngle), -sin(dividedAngle), 
					sin(dividedAngle), cos(dividedAngle));
	adjustable = offput * (texcoord - uCenter) + uCenter;
	adjustable = (adjustable - texcoord) * AR + texcoord;
	sumColor.rgb += texture(uTex, adjustable).rgb			* 15.f;
	dividedAngle += segmentSize;

	offput = mat2(	cos(dividedAngle), -sin(dividedAngle), 
					sin(dividedAngle), cos(dividedAngle));
	adjustable = offput * (texcoord - uCenter) + uCenter;
	adjustable = (adjustable - texcoord) * AR + texcoord;
	sumColor.rgb += texture(uTex, adjustable).rgb			* 20.f;
	dividedAngle += segmentSize;

	offput = mat2(	cos(dividedAngle), -sin(dividedAngle), 
					sin(dividedAngle), cos(dividedAngle));
	adjustable = offput * (texcoord - uCenter) + uCenter;
	adjustable = (adjustable - texcoord) * AR + texcoord;
	sumColor.rgb += texture(uTex, adjustable).rgb			* 15.f;
	dividedAngle += segmentSize;

	offput = mat2(	cos(dividedAngle), -sin(dividedAngle), 
					sin(dividedAngle), cos(dividedAngle));
	adjustable = offput * (texcoord - uCenter) + uCenter;
	adjustable = (adjustable - texcoord) * AR + texcoord;
	sumColor.rgb += texture(uTex, adjustable).rgb			* 6.f;
	dividedAngle += segmentSize;

	offput = mat2(	cos(dividedAngle), -sin(dividedAngle), 
					sin(dividedAngle), cos(dividedAngle));
	adjustable = offput * (texcoord - uCenter) + uCenter;
	adjustable = (adjustable - texcoord) * AR + texcoord;
	sumColor.rgb += texture(uTex, adjustable).rgb			* 1.f;
	dividedAngle += segmentSize;

	sumColor /= 64.f;

	outColor = sumColor;
}