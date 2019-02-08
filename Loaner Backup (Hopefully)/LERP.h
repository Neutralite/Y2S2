#pragma once
#include <vector>
#include "Jmath.h"

class LP
{
public:
	template<class TEMPLATE>
	static TEMPLATE LERP(TEMPLATE P1, TEMPLATE P2, float T) { return P1 * (1.0f - T) + P2 * T; }

	template<class TEMPLATE>
	static float INVLERP(TEMPLATE P1, TEMPLATE P2, TEMPLATE AT_POINT) { return (AT_POINT - P1).Length() / (P2 - P1).Length(); }

	static vec3 SLERP(vec3 P1, vec3 P2, vec3 AXIS, float T) {
		float ang = acos(Dot((P1 - AXIS).GetNormalized(), (P2 - AXIS).GetNormalized()));
		float nAng = T * ang;

		vec3 c1 = Cross(P1 - AXIS, P2 - AXIS).GetNormalized();
		vec3 c2 = Cross(c1, P1 - AXIS).GetNormalized() * (P2 - AXIS).Length();

		return (P1 - AXIS) * cos(nAng) + c2 * sin(nAng) + AXIS; 
	}

	template<class TEMPLATE>
	static TEMPLATE BEZIER(std::vector<TEMPLATE> ps, float T)
	{
		if (ps.size() > 2)
		{
			std::vector<TEMPLATE> ps1 = ps;
			std::vector<TEMPLATE> ps2 = ps;
			ps1.erase(ps1.begin());
			ps2.erase(ps1.end());
			return LERP(BEZIER(ps2, T), BEZIER(ps1, T), T);
		}
		else if (ps.size() == 2)
		{
			return LERP(ps[0], ps[1], T);
		}
		else
		{
			return 0;
		}
	}
};