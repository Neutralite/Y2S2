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
			//if (aPOINT.Length() > maxRad)
			//	maxRad = aPOINT.Length();
			if (length(aPOINT) > maxRad)
				maxRad = length(aPOINT);
		}
		else if (inputString[0] == 'f' && inputString[1] == ' ')
		{
			//int IN[CHAR_BUFFER_SIZE];
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

	for (int i = 0; i < (int)points.size(); i++)
	{
		//normals.push_back((Cross(points[(i + 1) % points.size()] - points[i], vec3(0, 1, 0))).GetNormalized());
		normals.push_back(normalize(cross(points[(i + 1) % points.size()] - points[i], vec3(0, 1, 0))));
	}

	enabled = true;
}

bool Hitbox2D::collidesWith(Hitbox2D * h, mat4 ownWorldMat, mat4 otherWorldMat)
{

	if (enabled && h->enabled)
	{
		if (length(ownWorldMat.translation() - otherWorldMat.translation()) > maxRad + h->maxRad)
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
			for (int i = 0; i < (int)points.size(); i++)
			{
				vec4 n1 = ownWorldMat * vec4(points[i], 1);
				vec4 n2 = ownWorldMat * vec4(normals[i], 0);
				pW1.push_back(n1.xyz);
				nW1.push_back(n2.xyz);
			}
			for (int i = 0; i < (int)h->points.size(); i++)
			{
				vec4 n1 = otherWorldMat * vec4(h->points[i], 1);
				vec4 n2 = otherWorldMat * vec4(h->normals[i], 0);
				pW2.push_back(n1.xyz);
				nW2.push_back(n2.xyz);
			}

			bool COLLIDE = true;

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

			closestPoint = vec3(0, 0, 0);
			int numOfClosest = 0;

			for (int i = 0; i < maxH; i++)
			{
				for (int j = 0; j < maxW; j++)
				{
					//std::cout << "PASS" << std::endl;
					if (dot(nW1[i], pW1[i] - pW2[j]) * dot(nW1[i], pW1[i] - pW2[(j + 1) % maxW]) < 0 && dot(nW2[j], pW2[j] - pW1[i]) * dot(nW2[j], pW2[j] - pW1[(i + 1) % maxH]) < 0)
					{
						float colDist = abs(dot(nW1[i], pW1[i] - pW2[j]));
						float totDist = colDist + abs(dot(nW1[i], pW1[i] - pW2[(j + 1) % maxW]));

						closestPoint *= (float) numOfClosest;
						closestPoint += lerp(pW2[j], pW2[(j + 1) % maxW], colDist / totDist);
						numOfClosest++;
						closestPoint /= (float) numOfClosest;

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
					if (dot(nW1[i], pW1[i] - pW2[0]) < 0)
					{
						YUP = false;
						//std::cout << "OHNO!" << std::endl;
					}
				}

				bool YUP2 = true;
				for (int i = 0; i < maxW; i++)
				{
					if (dot(nW2[i], pW2[i] - pW1[0]) < 0)
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
				if (length(closestPoint) == 0)
				{
					closestPoint = (ownWorldMat.translation() + otherWorldMat.translation()) * 0.5f;
				}

				vec4 sub = -otherWorldMat * vec4(0, 0, 0, 1) + ownWorldMat * vec4(0, 0, 0, 1);
				vec3 sub2 = sub.xyz;
				if (length(combinedNormal) <= 0.0001f)
				{
					combinedNormal = sub2;
					if (length(combinedNormal) <= 0.0001f)
					{
						combinedNormal = vec3(0, 0, -1);
						std::cout << "NORP" << std::endl;
					}
				}
				combinedNormal = normalize(combinedNormal);
				if (dot(combinedNormal, sub2) < 0)
					combinedNormal *= -1.f;

				//closestPoint = 0;
				//for (int i = 0; i < (int)pW1.size(); i++)
				//{
				//	if (dot(pW1[0] - pW1[i], combinedNormal) < dot(pW1[0] - pW1[closestPoint], combinedNormal))
				//	{
				//		closestPoint = i;
				//	}
				//}
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

std::string Hitbox2D::getName()
{
	return name;
}

void Hitbox2D::setName(std::string _NAME)
{
	name = _NAME;
}
