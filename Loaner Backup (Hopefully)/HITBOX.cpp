#define _CRT_SECURE_NO_WARNINGS
#include "HITBOX.h"
#include <fstream>

#define CHAR_BUFFER_SIZE 128

#define BUFFER_OFFSET(i) ((char *)0 + (i))

Hitbox::Hitbox()
{
}

Hitbox::~Hitbox()
{
}

void Hitbox::loadFromFile(std::string & fileName)
{
	std::ifstream input;
	input.open(fileName);
	
	char inputString[CHAR_BUFFER_SIZE];

	while (!input.eof())
	{
		input.getline(inputString, CHAR_BUFFER_SIZE);

		if (inputString[0] == 'v' && inputString[1] != 'n' && inputString[1] != 't')
		{
			vec3 pointOnHitbox;
			std::sscanf(inputString, "v %f %f %f", &pointOnHitbox.x, &pointOnHitbox.y, &pointOnHitbox.z);
			boxPoints.push_back(pointOnHitbox);
		}
		else if (inputString[0] == 'f')
		{
			std::vector<unsigned int> tempV;
			if (std::strstr(inputString, "\\") != nullptr)
			{
				float useless;
				int counter = 0;
				for (int i = 0; i < CHAR_BUFFER_SIZE; i++)
					if (inputString[i] == '/')
						counter++;
				counter /= 2;
				std::string sum = "f";
				for (int i = 0; i < counter; i++)
				{
					sum += " %u//%u";
					tempV.push_back(0);
				}
				if (counter == 3)
					std::sscanf(inputString, sum.c_str(), &tempV[0], &useless, &tempV[1], &useless, &tempV[2], &useless);
				else if (counter == 4)
					std::sscanf(inputString, sum.c_str(), &tempV[0], &useless, &tempV[1], &useless, &tempV[2], &useless, &tempV[3], &useless);
			}
			else
			{
				float useless;
				int counter = 0;
				for (int i = 0; i < CHAR_BUFFER_SIZE; i++)
					if (inputString[i] == '/')
						counter++;
				counter /= 2;
				std::string sum = "f";
				for (int i = 0; i < counter; i++)
				{
					sum += " %u/%u/%u";
					tempV.push_back(0);
				}
				if (counter == 3)
					std::sscanf(inputString, sum.c_str(), &tempV[0], &tempV[1], &tempV[2]);
				else if (counter == 4)
					std::sscanf(inputString, sum.c_str(), &tempV[0], &tempV[1], &tempV[2], &tempV[3]);
			}

			faces.push_back(tempV);
		}
	}

	for (int i = 0; i < faces.size(); i++)
	{
		int p = faces[i].size();
		for (int k = 0; k < p; k++)
		{
			bool hasIt = false;
			for (int j = 0; j < lines.size(); j++)
			{
				if (lines[j][0] == faces[i][k] && lines[j][1] == faces[i][(k + 1) % p])
				{
					hasIt = true;
				}
				else if (lines[j][1] == faces[i][k] && lines[j][0] == faces[i][(k + 1) % p])
				{
					hasIt = true;
				}
			}

			if (!hasIt)
			{
				std::vector<unsigned int> smol;
				smol.push_back(faces[i][k]);
				smol.push_back(faces[i][(k + 1) % p]);
				lines.push_back(smol);
			}
		}
	}

	for (int i = 0; i < faces.size(); i++)
	{
		faceNormals.push_back(Cross(boxPoints[faces[i][2]] - boxPoints[faces[i][1]], boxPoints[faces[i][0]] - boxPoints[faces[i][1]]));
		faceNormals[i] = faceNormals[i].GetNormalized();
	}

	input.close();
}

bool Hitbox::collidesWith(Hitbox * HB, mat4 ownWorldMat, mat4 otherWorldMat)
{
	if (enabled && HB->enabled)
	{
		std::vector<vec3> col1p;
		std::vector<vec3> col2p;
		std::vector<vec3> norm2;
		for (int i = 0; i < boxPoints.size(); i++)
		{
			vec4 worldSpace = ownWorldMat * vec4(boxPoints[i], 1);
			col1p.push_back(vec3(worldSpace.x, worldSpace.y, worldSpace.z));
		}
		for (int i = 0; i < HB->boxPoints.size(); i++)
		{
			vec4 worldSpace = otherWorldMat * vec4(HB->boxPoints[i], 1);
			col2p.push_back(vec3(worldSpace.x, worldSpace.y, worldSpace.z));
			vec4 worldNorm = otherWorldMat * vec4(HB->boxPoints[i], 0);
			norm2.push_back(vec3(worldNorm.x, worldNorm.y, worldNorm.z));
		}
		repulsionVector = vec3(0, 0, 0);
		repulsionMedian = vec3(0, 0, 0);
		int numberOfRepulsions = 0;
		bool isColliding = false;
		for (int i = 0; i < col2p.size(); i++)
		{
			for (int j = 0; j < lines.size(); j++)
			{
				if (Dot(col2p[HB->faces[i][0]] - col1p[lines[j][0]], norm2[i]) * Dot(col2p[HB->faces[i][0]] - col1p[lines[j][1]], norm2[i]) < 0.0f)
				{
					vec3 pointOnFace = col1p[lines[j][0]] - Dot(col1p[lines[j][0]] - col2p[HB->faces[i][0]], norm2[i]) * norm2[i];
					int p = HB->faces[i].size();
					for (int k = 0; k < p; i++)
					{
						if (Dot(Cross(pointOnFace - col2p[HB->faces[i][k]], col2p[HB->faces[i][(k + 1) % p]] - col2p[HB->faces[i][k]]), Cross(pointOnFace - col2p[HB->faces[i][(k + 1) % p]], col2p[HB->faces[i][(k + 2) % p]] - col2p[HB->faces[i][(k + 1) % p]])) < 0.f)
						{
							isColliding = true;
							numberOfRepulsions++;
							repulsionVector += norm2[i];
							repulsionMedian += pointOnFace;
							k = p;
						}
					}
				}
			}
		}

		if (isColliding)
		{
			repulsionVector = repulsionVector.GetNormalized();
			repulsionMedian /= numberOfRepulsions;

			vec4 subStep1 = ownWorldMat.GetInverse() * vec4(repulsionVector, 0);
			vec4 subStep2 = otherWorldMat.GetInverse() * vec4(repulsionMedian, 1);

			repulsionVector = vec3(subStep1.x, subStep1.y, subStep1.z);
			repulsionMedian = vec3(subStep2.x, subStep2.y, subStep2.z);
		}

		return isColliding;
	}
	else
	{
		return false;
	}
}

vec3 Hitbox::getMedianOfCollision()
{
	return repulsionMedian;
}

vec3 Hitbox::getRepulsionVector()
{
	return repulsionVector;
}

void Hitbox::setStatic(bool b)
{
	stationary = b;
}

bool Hitbox::isStatic()
{
	return stationary;
}

void Hitbox::setEnabled(bool b)
{
	enabled = b;
}

bool Hitbox::isEnabled()
{
	return enabled;
}
