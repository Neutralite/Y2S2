#pragma once
#include "GL\glew.h"

#include "Jmath.h"
#include <string>
#include <vector>

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	static bool initDefault();
	void setDefaultVert();
	void setDefaultFrag();
	void setDefaultProg();

	bool Load(const std::string &vertFile, const std::string &fragFile);
	bool reload();
	bool IsLoaded() const;
	void Unload();
	bool LinkProgram();

	void Bind() const;
	static void Unbind();

	void AddAttribute(unsigned int index, const std::string &attribName);

	int GetAttribLocation(const std::string &AttribName);
	int GetUniformLocation(const std::string &uniformName);

	void SendUniform(const std::string &name, int integer);
	void SendUniform(const std::string &name, unsigned int unsigned_integer);
	void SendUniform(const std::string &name, float scalar);
	void SendUniform(const std::string &name, const vec2 &vector);
	void SendUniform(const std::string &name, const vec3 &vector);
	void SendUniform(const std::string &name, const vec4 &vector);
	void SendUniformMat3(const std::string &name, float *matrix, bool transpose);
	void SendUniformMat4(const std::string &name, float *matrix, bool transpose);

	void SendUniformVectorVec4(const std::string & name, std::vector<vec4> &V);


private:
	std::string vertexFilename;
	std::string fragFilename;

	bool _IsInit = false;
	GLuint _VertexShader = 0;
	GLuint _FragmentShader = 0;
	GLuint _Program = 0;

	std::string ReadFile(const std::string &fileName) const;
	bool CompileShader(GLuint shader) const;
	void OutputShaderLog(GLuint shader) const;
	void OutputProgramLog() const;


};