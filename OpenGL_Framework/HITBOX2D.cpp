#define _CRT_SECURE_NO_WARNINGS
#include "HITBOX2D.h"
#include <fstream>
#include <iostream>
#include <string>
//#include <sstream>
#define CHAR_BUFFER_SIZE 128

#define BUFFER_OFFSET(i) ((char *)0 + (i))

Hitbox2D::Hitbox2D()
{
}

Hitbox2D::~Hitbox2D()
{
}

void Hitbox2D::loadFromFile(std::string & fileName)
{
	points.clear();
	normals.clear();
	std::ifstream input;
	input.open(fileName);

	char inputString[CHAR_BUFFER_SIZE];

	maxRad = 0.f;
	
	std::vector<vec3> tempPoints;
	while (!input.eof())
	{
		input.getline(inputString, CHAR_BUFFER_SIZE);

		if (inputString[0] == 'v' && inputString[1] == ' ')
		{
			vec3 aPOINT;
			std::sscanf(inputString, "v %f %f %f", &aPOINT.x, &aPOINT.y, &aPOINT.z);
			tempPoints.push_back(vec3(aPOINT.x, 0, aPOINT.z));
			if (aPOINT.Length() > maxRad)
				maxRad = aPOINT.Length();
		}
		else if (inputString[0] == 'f' && inputString[1] == ' ')
		{
			int IN[CHAR_BUFFER_SIZE];
			//int BAM = 0;
			//for (int i = 0; i < CHAR_BUFFER_SIZE; i++)
			//{
			//	if (inputString[i] == '/')
			//		BAM++;
			//	IN[i] = -1;
			//}
			//BAM /= 2;
			//std::string DIS = "f";
			//for (int i = 0; i < BAM; i++)
			//	DIS += " %d//%*d";
			//std::cout << DIS << std::endl;
			//std::sscanf(inputString, DIS.c_str(), IN);
			//for (int i = 0; i < CHAR_BUFFER_SIZE; i++)
			//{
			//	if (IN[i] >= 0)
			//	{
			//		points.push_back(tempPoints[IN[i]]);
			//	
			//	}
			//	else
			//	{
			//		i = CHAR_BUFFER_SIZE;
			//	}
			//}
			std::string parS = "";
			for (int i = 0; i < CHAR_BUFFER_SIZE; i++)
			{
				if (inputString[i] == ' ')
				{
					i++;
					bool ran = false;
					while (i < CHAR_BUFFER_SIZE && inputString[i] >= '0' && inputString[i] <= '9')
					{
						ran = true;
						parS += inputString[i];
						i++;
						//std::cout << parS << std::endl;
					}
					if (ran)
						points.push_back(tempPoints[std::stoi(parS) - 1]);
					parS = "";
				}
			}
			//std::cout << points.size() << std::endl;
		}
	}

	for (int i = 0; i < points.size(); i++)
	{
		normals.push_back((Cross(points[(i + 1) % points.size()] - points[i], vec3(0, 1, 0))).GetNormalized());
	}

	enabled = true;
}

bool Hitbox2D::collidesWith(Hitbox2D * h, mat4 * ownWorldMat, mat4 * otherWorldMat)
{

	if (enabled && h->enabled)
	{
		if ((*ownWorldMat * vec4(0, 0, 0, 1) - *otherWorldMat * vec4(0, 0, 0, 1)).Length() > maxRad + h->maxRad)
		{
			//std::cout << "aHA!" << std::endl;
			return false;
		}
		else
		{
			std::vector<vec3> pW1;
			std::vector<vec3> nW1;
			std::vector<vec3> pW2;
			std::vector<vec3> nW2;
			for (int i = 0; i < points.size(); i++)
			{
				vec4 n1 = *ownWorldMat * vec4(points[i], 1);
				vec4 n2 = *ownWorldMat * vec4(normals[i], 0);
				pW1.push_back(vec3(n1.x, n1.y, n1.z));
				nW1.push_back(vec3(n2.x, n2.y, n2.z));
			}
			for (int i = 0; i < h->points.size(); i++)
			{
				vec4 n1 = *otherWorldMat * vec4(h->points[i], 1);
				vec4 n2 = *otherWorldMat * vec4(h->normals[i], 0);
				pW2.push_back(vec3(n1.x, n1.y, n1.z));
				nW2.push_back(vec3(n2.x, n2.y, n2.z));
			}

			bool COLLIDE = true;

			//for (int i = 0; i < nW1.size(); i++)
			//{
			//	bool allMadeIt = true;
			//	for (int j = 0; j < pW2.size(); j++)
			//	{
			//		if (Dot(pW2[j] - pW1[i], nW1[i]) < 0)
			//			allMadeIt = false;
			//	}
			//	if (allMadeIt)
			//	{
			//		COLLIDE = false;
			//	}
			//}
			//
			//vec3 reboundNormal;
			//vec3 reboundPoint;
			//
			//for (int i = 0; i < nW2.size(); i++)
			//{
			//	bool allMadeIt = true;
			//	for (int j = 0; j < pW1.size(); j++)
			//	{
			//		if (Dot(pW1[j] - pW2[i], nW2[i]) < 0)
			//			allMadeIt = false;
			//	}
			//	if (allMadeIt)
			//	{
			//		COLLIDE = false;
			//	}
			//}

			int maxH = pW1.size();
			int maxW = pW2.size();
			COLLIDE = false;
			vec3 combinedNormal = vec3(0, 0, 0);
			std::vector<bool> am1;
			std::vector<bool> am2;

			for (int i = 0; i < maxH; i++)
			{
				am1.push_back(false);
			}
			for (int i = 0; i < maxW; i++)
			{
				am2.push_back(false);
			}
			int A1 = 0;
			int A2 = 0;

			for (int i = 0; i < maxH; i++)
			{
				for (int j = 0; j < maxW; j++)
				{
					//std::cout << "PASS" << std::endl;
					if (Dot(nW1[i], pW1[i] - pW2[j]) * Dot(nW1[i], pW1[i] - pW2[(j + 1) % maxW]) < 0 && Dot(nW2[j], pW2[j] - pW1[i]) * Dot(nW2[j], pW2[j] - pW1[(i + 1) % maxH]) < 0)
					{
						COLLIDE = true;
						if (!am1[i])
						{
							am1[i] = true;
							A1++;
						}
						if (!am2[j])
						{
							am2[j] = true;
							A2++;
						}
						//std::cout << "HOW MANY?" << std::endl;
					}
				}
			}
			
			//In case you forget, there's a problem right here. It shouldn't add all the normals, just the ones that matter. You had variables in place to deal with this, why the hell didn't you use them.
			if (A2 > A1)
			{
				for (int i = 0; i < maxH; i++)
					if (am1[i])
						combinedNormal += nW1[i];
			}
			else
			{
				for (int i = 0; i < maxW; i++)
					if (am2[i])
						combinedNormal += nW2[i];
			}

			if (!COLLIDE)
			{
				bool YUP = true;

				for (int i = 0; i < maxH; i++)
				{
					if (Dot(nW1[i], pW1[i] - pW2[0]) < 0)
					{
						YUP = false;
						//std::cout << "OHNO!" << std::endl;
					}
				}

				bool YUP2 = true;
				for (int i = 0; i < maxW; i++)
				{
					if (Dot(nW2[i], pW2[i] - pW1[0]) < 0)
					{
						YUP2 = false;
						//std::cout << "FAILED CHECK!" << std::endl;
					}
				}

				//if (YUP || YUP2)
				//	std::cout << "BOOP" << std::endl;
				COLLIDE = YUP || YUP2;
				//if (YUP || YUP2)
				//	std::cout << "YUP!" << std::endl;
			}

			if (COLLIDE)
			{
				vec4 sub = -*otherWorldMat * vec4(0, 0, 0, 1) + *ownWorldMat * vec4(0, 0, 0, 1);
				vec3 sub2 = vec3(sub.x, sub.y, sub.z);
				if (combinedNormal.Length() <= 0.0001f)
				{
					combinedNormal = sub2;
					if (combinedNormal.Length() <= 0.0001f)
					{
						combinedNormal = vec3(0, 0, -1);
						std::cout << "NORP" << std::endl;
					}
				}
				combinedNormal = combinedNormal.GetNormalized();
				if (Dot(combinedNormal, sub2) < 0)
					combinedNormal *= -1.f;

				closestPoint = 0;
				for (int i = 0; i < pW1.size(); i++)
				{
					if (Dot(pW1[0] - pW1[i], combinedNormal) < Dot(pW1[0] - pW1[closestPoint], combinedNormal))
					{
						closestPoint = i;
					}
				}
				outDir = combinedNormal;
			}
			//else
			//{
			//	std::cout << "AAAAAAAA!" << std::endl;
			//}

			return COLLIDE;
		}
	}
	else
	{
		return false;
	}
}
