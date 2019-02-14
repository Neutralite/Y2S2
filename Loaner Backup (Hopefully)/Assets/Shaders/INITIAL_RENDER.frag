#version 330

layout(location = 2) out vec3 outTexAlbedo;
layout(location = 3) out vec3 outTexSpecular;
layout(location = 0) out vec3 outLightDiffuse;
layout(location = 1) out vec3 outLightSpecular;
layout(location = 4) out vec3 unaffected;

uniform sampler2D uTex;

uniform vec4 position[24];
uniform vec4 color[24];
uniform vec4 direction[24];
uniform vec4 DSE[24];
uniform vec4 CLQ[24];
uniform float ambient;
uniform int lightType[24];
uniform int Lsize;

in vec3 normal;
in vec2 texcoord;
in vec3 pos;
in mat4 MVmat;

void main()
{
	vec3 N = normalize(normal);
	outLightDiffuse = vec3(0);
	outLightSpecular = vec3(0);
	outTexSpecular = vec3(1);

	for(int i = 0; i < 24 && i < Lsize; i++)
	{
		vec3 Lvec = -((MVmat * position[i]).xyz - pos);
		float dist = length(Lvec);
		float ATT = 1.0f / (CLQ[i].x + CLQ[i].y * dist + CLQ[i].z * dist * dist);

		if (lightType[i] == 0)
		{
			vec3 D = normalize((MVmat * direction[i]).xyz);

			float NdotL = -dot(D, N);
			
			if (NdotL > 0)
			{
				float NdotHV = max(dot(N, normalize(-D + normalize(-pos))), 0.0f);

				outLightDiffuse.rgb += DSE[i].x * NdotL * color[i].rgb;
				outLightSpecular.rgb += DSE[i].y * pow(NdotHV, DSE[i].z) * color[i].rgb;
			}
		}
		else if (lightType[i] == 1)
		{
			vec3 Ldir = Lvec / dist;

			float NdotL = -dot(Ldir, N);

			if (NdotL > 0)
			{
				float NdotHV = max(dot(N, normalize(Ldir + normalize(-pos))), 0.0f);

				outLightDiffuse.rgb += DSE[i].x * NdotL * ATT * color[i].rgb;
				outLightSpecular.rgb += DSE[i].y * pow(NdotHV, DSE[i].z) * ATT * color[i].rgb;
			}
		}
		else
		{
			vec3 Ldir = Lvec / dist;

			float NdotL = -dot(Ldir, N);

			vec3 D = normalize((MVmat * direction[i]).xyz);
			float isInside = -dot(Ldir, D);

			if (NdotL > 0 && isInside > cos(CLQ[i].w))
			{
				float NdotHV = max(dot(N, normalize(Ldir + normalize(-pos))), 0.0f);

				outLightDiffuse.rgb += DSE[i].x * NdotL * ATT * color[i].rgb;
				outLightSpecular.rgb += DSE[i].y * pow(NdotHV, DSE[i].z) * ATT * color[i].rgb;
			}
		}
		//float NdotL = dot(Ldir, N);
		//
		//
		//
		//if (NdotL > 0)
		//{
		//	float ATT = 1.0f / (CLQ[i].x + CLQ[i].y * dist + CLQ[i].z * dist * dist);
		//	float NdotHV = max(dot(N, normalize(Ldir + normalize(-pos))), 0.0f);
		//}
		//
		//if (NdotL > 0)
		//{
		//	float ATT = 1.0f / (Att_C + Att_L * dist + Att_Q * dist * dist);
		//
		//	outColor.rgb += Ldif * NdotL * ATT;
		//
		//	float NdotHV = max(dot(N, normalize(Ldir + normalize(-pos))), 0.0f);
		//
		//	outColor.rgb += Lspe * pow(NdotHV, Lexp) * ATT;
		//}
	}

	//outColor.rgb += vec3(ambient, ambient, ambient);
	vec4 texCol = texture(uTex, texcoord);
	outTexAlbedo.rgb = texCol.rgb;

	//if(dot(N, normalize(-pos)) < 0.1)
	//{
	//	outTexAlbedo.rgb = vec3(0);
	//	outTexSpecular.rgb = vec3(0);
	//	outLightDiffuse.rgb = vec3(0);
	//	outLightSpecular.rgb = vec3(0);
	//}
	//outTexAlbedo.a = texCol.a;
	unaffected = vec3(0);
}