#define _CRT_SECURE_NO_WARNINGS
#include "MESH.h"
#include <fstream>
#include <iostream>
#include "Jmath.h"

#define CHAR_BUFFER_SIZE 128

#define BUFFER_OFFSET(i) ((char *)0 + (i))

struct MeshFace
{
	MeshFace()
	{
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;


	}

	MeshFace(unsigned v1, unsigned v2, unsigned v3, unsigned t1, unsigned t2, unsigned t3, unsigned n1, unsigned n2, unsigned n3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
	}


	unsigned vertices[3];
	unsigned textureUVs[3];
	unsigned normals[3];
};

struct MeshFaceFloat
{
	MeshFaceFloat()
	{
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;

	}
	MeshFaceFloat(float v1, float v2, float v3, float v4,
		float t1, float t2, float t3, float t4,
		float n1, float n2, float n3, float n4)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;
		vertices[3] = v4;

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;
		textureUVs[3] = t4;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
		normals[3] = n4;
	}
	union
	{
		struct
		{
			float vertices[4];
			float textureUVs[4];
			float normals[4];
		};

		float face[16];
	};

};

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	Unload();
}

bool Mesh::LoadFromFile(const std::string & File)
{
	return LoadFromFileOFFSET(File, 0);
	
}

bool Mesh::LoadFromFileOFFSET(const std::string & File, int frame)
{
	std::ifstream input;
	input.open(File);
	if (!input)
	{
		std::cout << "FILE NOT LOADED" << std::endl;
		return false;
	}

	char inputString[CHAR_BUFFER_SIZE];

	//unique data
	std::vector<vec3> vertexData;
	std::vector<vec2> textureData;
	std::vector<vec3> normalData;

	//Index/Face Data
	std::vector<MeshFace> faceData;

	//OpenGL ready data
	std::vector<float> unPackedVertex;
	std::vector<float> unPackedTexture;
	std::vector<float> unPackedNormal;

	while (!input.eof())
	{
		input.getline(inputString, CHAR_BUFFER_SIZE);
		//std::cout << inputString << std::endl;

		if (std::strstr(inputString, "#") != nullptr)
		{
			//This line is commented out
			continue;
		}
		else if (std::strstr(inputString, "vt ") != nullptr)
		{
			vec2 temp;

			std::sscanf(inputString, "vt %f %f", &temp.x, &temp.y);
			textureData.push_back(temp);

			//Texture Data
		}
		else if (std::strstr(inputString, "vn ") != nullptr)
		{
			vec3 temp;

			std::sscanf(inputString, "vn %f %f %f", &temp.x, &temp.y, &temp.z);
			normalData.push_back(temp);

			//Normal Data
		}
		else if (std::strstr(inputString, "v ") != nullptr)
		{
			vec3 temp;

			std::sscanf(inputString, "v %f %f %f", &temp.x, &temp.y, &temp.z);
			vertexData.push_back(temp);

			//Vertex Data
		}
		else if (std::strstr(inputString, "f ") != nullptr)
		{
			//Face Data
			MeshFace temp;

			if (std::strstr(inputString, "//") != nullptr)
			{
				std::sscanf(inputString, "f %u//%u %u//%u %u//%u",
					&temp.vertices[0], &temp.normals[0],
					&temp.vertices[1], &temp.normals[1],
					&temp.vertices[2], &temp.normals[2]);
				temp.textureUVs[0] = 0;
				temp.textureUVs[1] = 0;
				temp.textureUVs[2] = 0;
			}
			else
			{
				std::sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
					&temp.vertices[0], &temp.textureUVs[0], &temp.normals[0],
					&temp.vertices[1], &temp.textureUVs[1], &temp.normals[1],
					&temp.vertices[2], &temp.textureUVs[2], &temp.normals[2]);
			}
			//std::cout << "\n" << temp.normals[1] << "\n" << std::endl;

			faceData.push_back(temp);
		}
	}

	input.close();


	//unpack the data
	for (unsigned int i = 0; i < faceData.size(); i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			//std::cout << faceData[i].vertices[j] << "/" << faceData[i].textureUVs[j] << "/" << faceData[i].normals[j] << std::endl;
			unPackedVertex.push_back(vertexData[faceData[i].vertices[j] - 1].x);
			unPackedVertex.push_back(vertexData[faceData[i].vertices[j] - 1].y);
			unPackedVertex.push_back(vertexData[faceData[i].vertices[j] - 1].z);

			if (faceData[i].textureUVs[j] == 0)
			{
				unPackedTexture.push_back(0);
				unPackedTexture.push_back(0);
			}
			else
			{
				unPackedTexture.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
				unPackedTexture.push_back(textureData[faceData[i].textureUVs[j] - 1].y);
			}

			unPackedNormal.push_back(normalData[faceData[i].normals[j] - 1].x);
			unPackedNormal.push_back(normalData[faceData[i].normals[j] - 1].y);
			unPackedNormal.push_back(normalData[faceData[i].normals[j] - 1].z);
		}
	}
	//std::cout << "AHA!" << std::endl;

	_NumFaces = faceData.size();
	_NumVertices = _NumFaces * 3;

	// Send da data!

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_Vertices);
	glGenBuffers(1, &VBO_UVs);
	glGenBuffers(1, &VBO_Normals);

	glBindVertexArray(VAO);

	//'Slots' for data
	glEnableVertexAttribArray(0 + frame * 3); //verts
	glEnableVertexAttribArray(1 + frame * 3); //uvs
	glEnableVertexAttribArray(2 + frame * 3); //normals

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedVertex.size(), &unPackedVertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0 + frame * 3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, reinterpret_cast<void*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedTexture.size(), &unPackedTexture[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1 + frame * 3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, reinterpret_cast<void*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedNormal.size(), &unPackedNormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2 + frame * 3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, reinterpret_cast<void*>(0));

	//Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	vertexData.clear();
	textureData.clear();
	normalData.clear();
	faceData.clear();
	unPackedVertex.clear();
	unPackedTexture.clear();
	unPackedNormal.clear();

	return true;
}

bool Mesh::LoadMultiMesh(const std::vector<std::string>& files)
{
	_NumFrames = files.size();

	VBO_Vert = new GLuint[_NumFrames];
	VBO_UV = new GLuint[_NumFrames];
	VBO_Norm = new GLuint[_NumFrames];
	glGenVertexArrays(1, &VAO);


	glBindVertexArray(VAO);

	for (unsigned int c = 0; c < _NumFrames; ++c)
	{
		glGenBuffers(1, &VBO_Vert[c]);
		glGenBuffers(1, &VBO_UV[c]);
		glGenBuffers(1, &VBO_Norm[c]);

		std::ifstream input;
		input.open(files[c]);

		if (!input)
		{
			std::cout << "Error: Could not open file \"" << files[c] << "\"!\n";
			//loaded = false;
			return false;

		}
		char line[CHAR_BUFFER_SIZE];

		vec3 topCorn; //Bounding box corner
		vec3 btmCorn; //Bounding box corner

		//Unique data
		std::vector<vec3> vertexData;
		std::vector<vec2> textureData;
		std::vector<vec3> normalData;
		//index/face data
		std::vector<MeshFace> faceData;

		float x, y, z;
		MeshFace faceTemp;

		//Use the face data to create an array of verts, uvs, and normals for faster use in openGL
		std::vector<float> unPackedVertexData;
		std::vector<float> unPackedTextureData;
		std::vector<float> unPackedNormalData;

		std::vector<MeshFaceFloat> unPackedFaceData;

		while (!input.eof()) //While the file has not ended, continue pulling in data.
		{
			input.getline(line, CHAR_BUFFER_SIZE);

			switch (line[0])
			{
			case 'v':
				switch (line[1])
				{
				case ' ':
					std::sscanf(line, "v %f %f %f", &x, &y, &z);
					vertexData.push_back(vec3(x, y, z));
					break;
				case 't':
					std::sscanf(line, "vt %f %f", &x, &y);
					textureData.push_back(vec2(x, y));
					break;
				case 'n':
					std::sscanf(line, "vn %f %f %f", &x, &y, &z);
					normalData.push_back(vec3(x, y, z));
					break;
				}
				break;
			case 'f':
			{
				int numSuccess = std::sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
					&faceTemp.vertices[0], &faceTemp.textureUVs[0], &faceTemp.normals[0],
					&faceTemp.vertices[1], &faceTemp.textureUVs[1], &faceTemp.normals[1],
					&faceTemp.vertices[2], &faceTemp.textureUVs[2], &faceTemp.normals[2]);
				if (numSuccess < 9)
				{
					numSuccess = std::sscanf(line, "f %u//%u %u//%u %u//%u",
						&faceTemp.vertices[0], &faceTemp.normals[0],
						&faceTemp.vertices[1], &faceTemp.normals[1],
						&faceTemp.vertices[2], &faceTemp.normals[2]);
					faceTemp.textureUVs[0] = 1;
					faceTemp.textureUVs[1] = 1;
					faceTemp.textureUVs[2] = 1;

					faceData.push_back(faceTemp);
					if (numSuccess < 6)
					{
						std::cout << "OH SHIT";
					}
				}
				else
				{
					faceData.push_back(faceTemp);
				}
			}
			break;
			case 'o':
			case '#':
			case 'm':
			default:
				break;
			}
		}
		input.close();

		if (textureData.size() < 2)
		{
			textureData.push_back(vec2());
		}

		//Unpack the data
		for (unsigned i = 0; i < faceData.size(); i++)
		{
			for (unsigned j = 0; j < 3; j++)
			{
				unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].x); //We use -1 because OBJ specs start at 1 instead of 0.
				unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].y);
				unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].z);
				unPackedVertexData.push_back(1.0f);

				unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
				unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].y);
				unPackedTextureData.push_back(0.0f);
				unPackedTextureData.push_back(1.0f);

				unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].x);
				unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].y);
				unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].z);
				unPackedNormalData.push_back(1.0f);
			}
		}

		unPackedFaceData.resize(faceData.size());

		_NumFaces = faceData.size();
		_NumVertices = _NumFaces * 3;

		unsigned int indexOffset = c * 3; // We have 3 VBOs

		glEnableVertexAttribArray(0 + indexOffset);
		glEnableVertexAttribArray(1 + indexOffset);
		glEnableVertexAttribArray(2 + indexOffset);

		//Vertex
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Vert[c]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedVertexData.size(), &unPackedVertexData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0 + indexOffset, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, BUFFER_OFFSET(0));

		//UVs
		glBindBuffer(GL_ARRAY_BUFFER, VBO_UV[c]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedTextureData.size(), &unPackedTextureData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1 + indexOffset, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, BUFFER_OFFSET(0));

		//Normals
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Norm[c]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedNormalData.size(), &unPackedNormalData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)2 + indexOffset, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, BUFFER_OFFSET(0));


		vertexData.clear(); //Clear the vectors from RAM now that everything's in the GPU.
		textureData.clear();
		normalData.clear();
		faceData.clear();
		unPackedVertexData.clear();
		unPackedTextureData.clear();
		unPackedNormalData.clear();
		unPackedFaceData.clear();
	}
	//Begin cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void Mesh::Unload()
{
	glDeleteBuffers(1, &VBO_Normals);
	glDeleteBuffers(1, &VBO_UVs);
	glDeleteBuffers(1, &VBO_Vertices);

	//glDeleteVertexArrays(1, &VAO);

	VBO_Normals = 0;
	VBO_UVs = 0;
	VBO_Vertices = 0;
	VAO = 0;

	_NumFaces = 0;
	_NumVertices = 0;

	for (int c = 0; c < _NumFrames; ++c)
	{
		glDeleteBuffers(1, &VBO_Norm[c]);
		glDeleteBuffers(1, &VBO_UV[c]);
		glDeleteBuffers(1, &VBO_Vert[c]);
	}
	glDeleteVertexArrays(1, &VAO);
	VBO_Norm = nullptr;
	VBO_UV = nullptr;
	VBO_Vert = nullptr;
	VAO = 0;

	_NumFaces = 0;
	_NumVertices = 0;
	_NumFrames = 0;
}

unsigned int Mesh::GetNumFaces() const
{
	return _NumFaces;
}

unsigned int Mesh::GetNumVertices() const
{
	return _NumVertices;
}
