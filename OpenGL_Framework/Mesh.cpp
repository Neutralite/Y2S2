#define _CRT_SECURE_NO_WARNINGS //Remove warnings from deprecated functions. Shut up, Microsoft.
#define CHAR_BUFFER_SIZE 128 //Size of the character buffer.

#include "Mesh.h"

#include <vector>
#include <fstream>
#include <iostream>

std::string Mesh::_ModelDirectory = "../assets/models/";

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

	MeshFace(unsigned v1, unsigned v2, unsigned v3,
		unsigned u1, unsigned u2, unsigned u3,
		unsigned n1, unsigned n2, unsigned n3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;

		textureUVs[0] = u1;
		textureUVs[1] = u2;
		textureUVs[2] = u3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
	}

	unsigned vertices[3];
	unsigned textureUVs[3];
	unsigned normals[3];
	unsigned tangent[3];
	unsigned bitangent[3];
};

void Mesh::initMeshSphere(const unsigned int xSlices, const unsigned int ySlices, bool invert)
{
	float x = (float)xSlices;
	float y = (float)ySlices;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			float s0 = sin(((j + 0) / y)* f_pi);
			float s1 = sin(((j + 1) / y)* f_pi);

			float c0 = cos((2.0f * (j + 0) / y - 1.0f)* f_pi * 0.5f + f_pi * 0.5f);
			float c1 = cos((2.0f * (j + 1) / y - 1.0f)* f_pi * 0.5f + f_pi * 0.5f);

			vec4 blPos = vec4(s0 * sin((i + 0) * f_pi*2.0f / x), c0, s0 * cos((i + 0) * f_pi*2.0f / x), 0.0f);
			vec4 brPos = vec4(s0 * sin((i + 1) * f_pi*2.0f / x), c0, s0 * cos((i + 1) * f_pi*2.0f / x), 0.0f);
			vec4 tlPos = vec4(s1 * sin((i + 0) * f_pi*2.0f / x), c1, s1 * cos((i + 0) * f_pi*2.0f / x), 0.0f);
			vec4 trPos = vec4(s1 * sin((i + 1) * f_pi*2.0f / x), c1, s1 * cos((i + 1) * f_pi*2.0f / x), 0.0f);

			//s0 = 1.0f;
			//s1 = 1.0f;
			if (!invert)
			{
				dataVertex.push_back(blPos);
				dataVertex.push_back(tlPos);
				dataVertex.push_back(brPos);
				dataVertex.push_back(tlPos);
				dataVertex.push_back(trPos);
				dataVertex.push_back(brPos);
			}
			else
			{
				dataVertex.push_back(blPos);
				dataVertex.push_back(brPos);
				dataVertex.push_back(tlPos);
				dataVertex.push_back(tlPos);
				dataVertex.push_back(brPos);
				dataVertex.push_back(trPos);
			}

			vec4 bl = vec4((i) / x, 1.f - (j) / y, 0.f, 0.f);
			vec4 br = vec4((i + 1) / x, 1.f - (j) / y, 0.f, 0.f);
			vec4 tl = vec4((i) / x, 1.f - (j + 1) / y, 0.f, 0.f);
			vec4 tr = vec4((i + 1) / x, 1.f - (j + 1) / y, 0.f, 0.f);

			if (!invert)
			{
				dataTexture.push_back(bl);
				dataTexture.push_back(tl);
				dataTexture.push_back(br);
				dataTexture.push_back(tl);
				dataTexture.push_back(tr);
				dataTexture.push_back(br);
			}
			else
			{
				dataTexture.push_back(bl);
				dataTexture.push_back(br);
				dataTexture.push_back(tl);
				dataTexture.push_back(tl);
				dataTexture.push_back(br);
				dataTexture.push_back(tr);
			}

			if (!invert)
			{
				dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 6]));
				dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 5]));
				dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 4]));
				dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 3]));
				dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 2]));
				dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 1]));
			}
			else
			{
				dataNormal.push_back(-normalize(dataVertex[dataVertex.size() - 6]));
				dataNormal.push_back(-normalize(dataVertex[dataVertex.size() - 5]));
				dataNormal.push_back(-normalize(dataVertex[dataVertex.size() - 4]));
				dataNormal.push_back(-normalize(dataVertex[dataVertex.size() - 3]));
				dataNormal.push_back(-normalize(dataVertex[dataVertex.size() - 2]));
				dataNormal.push_back(-normalize(dataVertex[dataVertex.size() - 1]));
			}

		}
	}

	amntOfSpace = 0;
	uploadMatsToGPU();
	//uploadToGPU();
}

void Mesh::initMeshPlane(const unsigned int xDivisions, const unsigned int yDivisions)
{
	float x = (float)xDivisions;
	float y = (float)yDivisions;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			dataVertex.push_back(vec4((i + 0) / x * 2 - 1, 0.0f, (j + 0) / y * 2 - 1, 0.0f));
			dataVertex.push_back(vec4((i + 0) / x * 2 - 1, 0.0f, (j + 1) / y * 2 - 1, 0.0f));
			dataVertex.push_back(vec4((i + 1) / x * 2 - 1, 0.0f, (j + 0) / y * 2 - 1, 0.0f));
			dataVertex.push_back(vec4((i + 0) / x * 2 - 1, 0.0f, (j + 1) / y * 2 - 1, 0.0f));
			dataVertex.push_back(vec4((i + 1) / x * 2 - 1, 0.0f, (j + 1) / y * 2 - 1, 0.0f));
			dataVertex.push_back(vec4((i + 1) / x * 2 - 1, 0.0f, (j + 0) / y * 2 - 1, 0.0f));

			vec4 bl = vec4((i) / x, 1.f - (j) / y, 0.f, 0.f);
			vec4 br = vec4((i + 1) / x, 1.f - (j) / y, 0.f, 0.f);
			vec4 tl = vec4((i) / x, 1.f - (j + 1) / y, 0.f, 0.f);
			vec4 tr = vec4((i + 1) / x, 1.f - (j + 1) / y, 0.f, 0.f);

			dataTexture.push_back(bl);
			dataTexture.push_back(tl);
			dataTexture.push_back(br);
			dataTexture.push_back(tl);
			dataTexture.push_back(tr);
			dataTexture.push_back(br);

			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
		}
	}

	amntOfSpace = 0;
	uploadMatsToGPU();
	//uploadToGPU();
}

bool Mesh::LoadFromObj(const std::string & file, unsigned int makeInstances)
{
	std::ifstream input;
	input.open(_ModelDirectory + file);

	if (!input)
	{
		SAT_ERROR_LOC("Error: Could not open file \"%s\"!\n", file.c_str());
		_IsLoaded = false;
		return false;
	}
	char line[CHAR_BUFFER_SIZE];

	vec3 topCorn; // Bounding box corner
	vec3 btmCorn; // Bounding box corner

	//Unique data
	std::vector<vec3> vertexDataLoad;
	std::vector<vec2> textureDataLoad;
	std::vector<vec3> normalDataLoad;

	// Index/Face data
	std::vector<MeshFace> faceData;

	float x, y, z;
	MeshFace faceTemp;
	
	while (!input.eof()) // Go to end of file
	{
		input.getline(line, CHAR_BUFFER_SIZE);

		switch (line[0])
		{
		case 'v':
			switch (line[1])
			{
			case ' ':
				std::sscanf(line, "v %f %f %f", &x, &y, &z);
				vertexDataLoad.push_back(vec3(x, y, z));				
				break;
			case 't':
				std::sscanf(line, "vt %f %f", &x, &y);
				textureDataLoad.push_back(vec2(x, y));
				break;
			case 'n':
				std::sscanf(line, "vn %f %f %f", &x, &y, &z);
				normalDataLoad.push_back(vec3(x, y, z));
				break;
			}
			break;
		case 'f':
			std::sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&faceTemp.vertices[0], &faceTemp.textureUVs[0], &faceTemp.normals[0],
				&faceTemp.vertices[1], &faceTemp.textureUVs[1], &faceTemp.normals[1],
				&faceTemp.vertices[2], &faceTemp.textureUVs[2], &faceTemp.normals[2]);
			faceData.push_back(faceTemp);
			break;
		case 'o':
		case '#':
		case 'm':
		default:
			break;
		}
	}
	input.close();

	//Unpack the data
	for (unsigned i = 0; i < faceData.size(); i++)
	{
		for (unsigned j = 0; j < 3; j++)
		{
			vec4 data = vec4(
				vertexDataLoad[faceData[i].vertices[j] - 1].x,
				vertexDataLoad[faceData[i].vertices[j] - 1].y,
				vertexDataLoad[faceData[i].vertices[j] - 1].z, 
				1.0f);
			dataVertex.push_back(vec4(
				vertexDataLoad[faceData[i].vertices[j] - 1].x,
				vertexDataLoad[faceData[i].vertices[j] - 1].y,
				vertexDataLoad[faceData[i].vertices[j] - 1].z,
				1.0f));

			dataTexture.push_back(vec4(
				textureDataLoad[faceData[i].textureUVs[j] - 1].x,
				textureDataLoad[faceData[i].textureUVs[j] - 1].y,
				0.0f,
				1.0f));
			
			dataNormal.push_back(vec4(
				normalDataLoad[faceData[i].normals[j] - 1].x,
				normalDataLoad[faceData[i].normals[j] - 1].y,
				normalDataLoad[faceData[i].normals[j] - 1].z,
				1.0f));
		}



	}
	amntOfSpace = makeInstances;

	uploadMatsToGPU();
	//uploadToGPU();

	vertexDataLoad.clear(); //Clear the vectors from RAM now that everything's in the GPU.
	textureDataLoad.clear();
	normalDataLoad.clear();
	faceData.clear();
	return true;
}



void Mesh::bind() const
{
	MATS.bind();
	//vao.bind();
}

void Mesh::unbind()
{
	glBindVertexArray(GL_NONE);
}

void Mesh::insertMatrixAndSway(mat4 _M, vec4 _S)
{
	objLocs[usedSpace] = _M;
	SWAY[usedSpace] = _S;
}

std::string Mesh::getName()
{
	return ID;
}

void Mesh::setName(std::string _ID)
{
	ID = _ID;
}

void Mesh::draw() const
{
	MATS.draw();
	//vao.draw();
}

void Mesh::dynamicDraw()
{
	MATS.reuploadVAO();
	MATS.dynamicDraw(usedSpace);
}

void Mesh::uploadToGPU()
{
	unsigned int numTris = (unsigned int)(dataVertex.size() / 3);

	if (dataVertex.size() > 0)
	{
		VertexBufferData posAttrib;
		posAttrib.attributeType = AttributeLocations::VERTEX;
		posAttrib.data = &dataVertex[0];
		posAttrib.sizeOfElement = sizeof(float);
		posAttrib.elementType = GL_FLOAT;
		posAttrib.numElementsPerAttribute = 4;
		posAttrib.numElements = numTris * 3 * posAttrib.numElementsPerAttribute;
		vao.addVBO(posAttrib);
	}

	if (dataTexture.size() > 0)
	{
		VertexBufferData textureAttrib;
		textureAttrib.attributeType = AttributeLocations::TEXCOORD;
		textureAttrib.data = &dataTexture[0];
		textureAttrib.sizeOfElement = sizeof(float);
		textureAttrib.elementType = GL_FLOAT;
		textureAttrib.numElementsPerAttribute = 4;
		textureAttrib.numElements = numTris * 3 * textureAttrib.numElementsPerAttribute;
		vao.addVBO(textureAttrib);
	}

	if (dataNormal.size() > 0)
	{
		VertexBufferData normalAttrib;
		normalAttrib.attributeType = AttributeLocations::NORMAL;
		normalAttrib.data = &dataNormal[0];
		normalAttrib.sizeOfElement = sizeof(float);
		normalAttrib.elementType = GL_FLOAT;
		normalAttrib.numElementsPerAttribute = 4;
		normalAttrib.numElements = numTris * 3 * normalAttrib.numElementsPerAttribute;
		vao.addVBO(normalAttrib);
	}

	if (dataColor.size() > 0)
	{
		VertexBufferData colorAttrib;
		colorAttrib.attributeType = AttributeLocations::COLOR;
		colorAttrib.data = &dataColor[0];
		colorAttrib.sizeOfElement = sizeof(float);
		colorAttrib.elementType = GL_FLOAT;
		colorAttrib.numElementsPerAttribute = 4;
		colorAttrib.numElements = numTris * 3 * colorAttrib.numElementsPerAttribute;
		vao.addVBO(colorAttrib);
	}

	vao.createVAO();
	_IsLoaded = true;
}

void Mesh::uploadMatsToGPU()
{
	objLocs.resize(amntOfSpace);
	SWAY.resize(amntOfSpace);

	int numMats = amntOfSpace;
	if (numMats > 0)
	{
		//std::cout << "YAYAYA!" << std::endl;
		VertexBufferData matAttrib1;
		matAttrib1.attributeType = AttributeLocations::INSTANCED_COL_0;
		matAttrib1.data = &objLocs[0];
		matAttrib1.sizeOfElement = sizeof(float);
		matAttrib1.elementType = GL_FLOAT;
		matAttrib1.numElementsPerAttribute = 4;
		matAttrib1.numElements = amntOfSpace * matAttrib1.numElementsPerAttribute;
		matAttrib1.instance = GL_TRUE;
		MATS.addVBO(matAttrib1);

		VertexBufferData matAttrib2;
		matAttrib2.attributeType = AttributeLocations::INSTANCED_COL_1;
		matAttrib2.data = &objLocs[0];
		matAttrib2.sizeOfElement = sizeof(float);
		matAttrib2.elementType = GL_FLOAT;
		matAttrib2.numElementsPerAttribute = 4;
		matAttrib2.numElements = amntOfSpace * matAttrib2.numElementsPerAttribute;
		matAttrib2.instance = GL_TRUE;
		MATS.addVBO(matAttrib2);

		VertexBufferData matAttrib3;
		matAttrib3.attributeType = AttributeLocations::INSTANCED_COL_2;
		matAttrib3.data = &objLocs[0];
		matAttrib3.sizeOfElement = sizeof(float);
		matAttrib3.elementType = GL_FLOAT;
		matAttrib3.numElementsPerAttribute = 4;
		matAttrib3.numElements = amntOfSpace * matAttrib3.numElementsPerAttribute;
		matAttrib3.instance = GL_TRUE;
		MATS.addVBO(matAttrib3);

		VertexBufferData matAttrib4;
		matAttrib4.attributeType = AttributeLocations::INSTANCED_COL_3;
		matAttrib4.data = &objLocs[0];
		matAttrib4.sizeOfElement = sizeof(float);
		matAttrib4.elementType = GL_FLOAT;
		matAttrib4.numElementsPerAttribute = 4;
		matAttrib4.numElements = amntOfSpace * matAttrib4.numElementsPerAttribute;
		matAttrib4.instance = GL_TRUE;
		MATS.addVBO(matAttrib4);

		VertexBufferData swayAttrib;
		swayAttrib.attributeType = AttributeLocations::INSTANCED_SWAY;
		swayAttrib.data = &SWAY[0];
		swayAttrib.sizeOfElement = sizeof(float);
		swayAttrib.elementType = GL_FLOAT;
		swayAttrib.numElementsPerAttribute = 4;
		swayAttrib.numElements = amntOfSpace * swayAttrib.numElementsPerAttribute;
		swayAttrib.instance = GL_TRUE;
		MATS.addVBO(swayAttrib);
	}

	unsigned int numTris = (unsigned int)(dataVertex.size() / 3);

	if (dataVertex.size() > 0)
	{
		VertexBufferData posAttrib;
		posAttrib.attributeType = AttributeLocations::VERTEX;
		posAttrib.data = &dataVertex[0];
		posAttrib.sizeOfElement = sizeof(float);
		posAttrib.elementType = GL_FLOAT;
		posAttrib.numElementsPerAttribute = 4;
		posAttrib.numElements = numTris * 3 * posAttrib.numElementsPerAttribute;
		MATS.addVBO(posAttrib);
	}

	if (dataTexture.size() > 0)
	{
		VertexBufferData textureAttrib;
		textureAttrib.attributeType = AttributeLocations::TEXCOORD;
		textureAttrib.data = &dataTexture[0];
		textureAttrib.sizeOfElement = sizeof(float);
		textureAttrib.elementType = GL_FLOAT;
		textureAttrib.numElementsPerAttribute = 4;
		textureAttrib.numElements = numTris * 3 * textureAttrib.numElementsPerAttribute;
		MATS.addVBO(textureAttrib);
	}

	if (dataNormal.size() > 0)
	{
		VertexBufferData normalAttrib;
		normalAttrib.attributeType = AttributeLocations::NORMAL;
		normalAttrib.data = &dataNormal[0];
		normalAttrib.sizeOfElement = sizeof(float);
		normalAttrib.elementType = GL_FLOAT;
		normalAttrib.numElementsPerAttribute = 4;
		normalAttrib.numElements = numTris * 3 * normalAttrib.numElementsPerAttribute;
		MATS.addVBO(normalAttrib);
	}

	if (dataColor.size() > 0)
	{
		VertexBufferData colorAttrib;
		colorAttrib.attributeType = AttributeLocations::COLOR;
		colorAttrib.data = &dataColor[0];
		colorAttrib.sizeOfElement = sizeof(float);
		colorAttrib.elementType = GL_FLOAT;
		colorAttrib.numElementsPerAttribute = 4;
		colorAttrib.numElements = numTris * 3 * colorAttrib.numElementsPerAttribute;
		MATS.addVBO(colorAttrib);
	}

	//MATS.createVAO();

	if (numMats > 0)
		MATS.createVAO(GL_DYNAMIC_DRAW);
	else
		MATS.createVAO();
	_IsLoaded = true;
}
