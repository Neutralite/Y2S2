#pragma once
#include "GL/glew.h"
#include "Transform.h"
#include <vector>
#include "VertexBufferObject.h"
#include <string>

class Mesh
{
public:
	void initMeshSphere(const unsigned int xSlices, const unsigned int ySlices, bool invert = false);
	void initMeshPlane(const unsigned int xDivisions, const unsigned int yDivisions);
	
	bool LoadFromObj(const std::string &file, unsigned int makeInstances = 0);

	std::vector<vec4> dataVertex;
	std::vector<vec4> dataTexture;
	std::vector<vec4> dataNormal;
	std::vector<vec4> dataColor;

	void draw() const;
	void dynamicDraw();
	void Mesh::bind() const;
	static void Mesh::unbind();
	void insertMatrixAndSway(mat4 _M, vec4 _S);

	std::string getName();
	void setName(std::string _ID);

	std::vector<mat4> objLocs;
	std::vector<vec4> SWAY;

	unsigned int amntOfSpace;
	unsigned int usedSpace = 0;
private:

	std::string ID;
	VertexArrayObject vao;

	VertexArrayObject MATS;

	bool _IsLoaded = false;
	static std::string _ModelDirectory;

	void uploadToGPU();
	void uploadMatsToGPU();
};