#version 330

out vec4 outColor;

uniform sampler2D uTex;

uniform vec4 Lpos;

uniform vec3 Lamb;
uniform vec3 Ldif;
uniform vec3 Lspe;

uniform float Lexp;
uniform float Att_C;
uniform float Att_L;
uniform float Att_Q;

in vec3 normal;
in vec2 texcoord;
in vec3 pos;

void main()
{
	outColor.rgb = Lamb;

	vec3 N = normalize(normal);

	vec3 Lvec = Lpos.xyz - pos;
	float dist = length(Lvec);
	vec3 Ldir = Lvec / dist;

	float NdotL = dot(Ldir, N);

	if (NdotL > 0)
	{
		float ATT = 1.0f / (Att_C + Att_L * dist + Att_Q * dist * dist);

		outColor.rgb += Ldif * NdotL * ATT;

		float NdotHV = max(dot(N, normalize(Ldir + normalize(-pos))), 0.0f);

		outColor.rgb += Lspe * pow(NdotHV, Lexp) * ATT;
	}
	vec4 texCol  = texture(uTex, texcoord);
	outColor.rgb *= texCol.rgb;
	outColor.a = texCol.a;
}