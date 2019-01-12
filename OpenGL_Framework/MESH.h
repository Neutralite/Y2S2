#pragma once
#include <string>
#include <vector>
#include <GL\glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

	// Load The Data/Send to OpenGL
	bool LoadFromFile(const std::string &File);
	bool LoadFromFileOFFSET(const std::string &File, int frame);
	bool LoadMultiMesh(const std::vector<std::string> &files);
	// Release The Data
	void Unload();

	unsigned int GetNumFaces() const;
	unsigned int GetNumVertices() const;

	unsigned int _NumFaces = 0;
	unsigned int _NumVertices = 0;

	unsigned int _NumFrames = 0;

	GLuint *VBO_Vert = 0;
	GLuint *VBO_UV = 0;
	GLuint *VBO_Norm = 0;

	//OpenGl buffers and objs
	std::string ID;
	GLuint VBO_Vertices = 0;
	GLuint VBO_UVs = 0;
	GLuint VBO_Normals = 0;
	GLuint VAO = 0;
};