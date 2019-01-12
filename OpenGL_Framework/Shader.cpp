#include "Shader.h"

#include <vector>
#include <fstream>
#include <iostream>
#include<cassert>

bool _IsInitDefault = false;
GLuint _VertexShaderDefault = 0;
GLuint _FragmentShaderDefault = 0;
GLuint _ProgramDefault = 0;

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
	if (_IsInit)
	{
		Unload();
	}
}

bool ShaderProgram::initDefault()
{
	if (!_IsInitDefault)
	{
		ShaderProgram errorShader;
		bool compileSuccess = errorShader.Load("Assets/Shaders/ERROR.vert", "Assets/Shaders/ERROR.frag");
		if (!compileSuccess)
		{
			std::cout << "Could not compile da shader! Something wrong amigo!";
			system("PAUSE");
			exit(EXIT_FAILURE);
		}
		_VertexShaderDefault = errorShader._VertexShader;
		_FragmentShaderDefault = errorShader._FragmentShader;
		_ProgramDefault = errorShader._Program;
		_IsInitDefault = true;
	}
	return _IsInitDefault;
}

void ShaderProgram::setDefaultVert()
{
	_VertexShader = _VertexShaderDefault;
}

void ShaderProgram::setDefaultFrag()
{
	_FragmentShader = _FragmentShaderDefault;
}

void ShaderProgram::setDefaultProg()
{
	_Program = _ProgramDefault;
}

bool ShaderProgram::Load(const std::string & vertFile, const std::string & fragFile)
{
	vertexFilename = vertFile;
	fragFilename = fragFile;
	_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	_Program = glCreateProgram();

	std::string source = ReadFile(vertFile);
	const GLchar *temp = static_cast<const GLchar *>(source.c_str());
	glShaderSource(_VertexShader, 1, &temp, NULL);

	source = ReadFile(fragFile);
	temp = static_cast<const GLchar *>(source.c_str());
	glShaderSource(_FragmentShader, 1, &temp, NULL);

	_IsInit = true;

	if (!CompileShader(_VertexShader))
	{
		std::cout << "vertex shader failed! OH NO!\n";

		OutputShaderLog(_VertexShader);
		Unload();

		setDefaultVert();

		_IsInit = false;
	}
	
	if (!CompileShader(_FragmentShader))
	{
		std::cout << "fragment shader failed! OH NO!\n";

		OutputShaderLog(_FragmentShader);
		Unload();

		setDefaultFrag();

		_IsInit = false;
	}

	glAttachShader(_Program, _VertexShader);
	glAttachShader(_Program, _FragmentShader);

	if (!LinkProgram())
	{
		std::cout << "Shader Program loadn't.\n" << vertFile << "\n" << fragFile << "\n" << std::endl;

		OutputProgramLog();
		Unload();

		setDefaultProg();

		_IsInit = false;
	}

	return true;
}

bool ShaderProgram::reload()
{
	Unload();
	return Load(vertexFilename, fragFilename);
}

bool ShaderProgram::IsLoaded() const
{
	return _IsInit;
}

void ShaderProgram::Unload()
{
	if (_VertexShader != 0 && _VertexShader != _VertexShaderDefault)
	{
		glDetachShader(_Program, _VertexShader);
		glDeleteShader(_VertexShader);
		_VertexShader = 0;
	}

	if (_FragmentShader != 0 && _FragmentShader != _FragmentShaderDefault)
	{
		glDetachShader(_Program, _FragmentShader);
		glDeleteShader(_FragmentShader);
		_FragmentShader = 0;
	}

	if (_Program != 0 && _Program != _ProgramDefault)
	{
		glDeleteProgram(_Program);
		_Program = 0;
	}

	_IsInit = false;
}

bool ShaderProgram::LinkProgram()
{
	glLinkProgram(_Program);

	GLint success;
	glGetProgramiv(_Program, GL_LINK_STATUS, &success);

	return success == GL_TRUE;
}

void ShaderProgram::Bind() const
{
	glUseProgram(_Program);
}

void ShaderProgram::Unbind()
{
	glUseProgram(0);
}

void ShaderProgram::AddAttribute(unsigned int index, const std::string & attribName)
{
	glBindAttribLocation(_Program, index, attribName.c_str());
}

int ShaderProgram::GetAttribLocation(const std::string & AttribName)
{
	return glGetAttribLocation(_Program, AttribName.c_str());
}

int ShaderProgram::GetUniformLocation(const std::string & uniformName)
{
	GLint uniLoc = glGetUniformLocation(_Program, uniformName.c_str());
#ifdef _DEBUG
	if (uniLoc == -1 && _IsInit)
		std::cout << "WARNIN! You ain't got no " << uniformName << " here!" << std::endl;
#endif
	return uniLoc;
}

void ShaderProgram::SendUniform(const std::string & name, int integer)
{
	GLint location = GetUniformLocation(name);
	glUniform1i(location, integer);
}

void ShaderProgram::SendUniform(const std::string & name, unsigned int unsigned_integer)
{
	GLint location = GetUniformLocation(name);
	glUniform1ui(location, unsigned_integer);
}

void ShaderProgram::SendUniform(const std::string & name, float scalar)
{
	GLint location = GetUniformLocation(name);
	glUniform1f(location, scalar);
}

void ShaderProgram::SendUniform(const std::string & name, const vec2 & vector)
{
	GLint location = GetUniformLocation(name);
	glUniform2f(location, vector.x, vector.y);
}

void ShaderProgram::SendUniform(const std::string & name, const vec3 & vector)
{
	GLint location = GetUniformLocation(name);
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::SendUniform(const std::string & name, const vec4 & vector)
{
	GLint location = GetUniformLocation(name);
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void ShaderProgram::SendUniformMat3(const std::string & name, float * matrix, bool transpose)
{
	GLint location = GetUniformLocation(name);
	glUniformMatrix3fv(location, 1, transpose, matrix);
}

void ShaderProgram::SendUniformMat4(const std::string & name, float * matrix, bool transpose)
{
	GLint location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, matrix);
}

void ShaderProgram::SendUniformVectorVec4(const std::string & name, std::vector<vec4> &V)
{
	GLuint location = GetUniformLocation(name);
	glUniform4fv(location, (unsigned int) V.size(), reinterpret_cast<float *>(V.data()));
}

std::string ShaderProgram::ReadFile(const std::string & fileName) const
{
	std::ifstream inStream(fileName);
	if (!inStream.good())
	{
		std::cout << "Shader File 404 - not found" << std::endl;
		return std::string();
	}

	std::string data(std::istreambuf_iterator<char>(inStream), (std::istreambuf_iterator<char>()));

	return data;
}

bool ShaderProgram::CompileShader(GLuint shader) const
{
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	return success == GL_TRUE;
}

void ShaderProgram::OutputShaderLog(GLuint shader) const
{
	std::vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
	glGetShaderInfoLog(shader, sizeof(char) * 512, &infoLen, &infoLog[0]);

	std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
}

void ShaderProgram::OutputProgramLog() const
{
	std::vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetProgramiv(_Program, GL_INFO_LOG_LENGTH, &infoLen);
	glGetProgramInfoLog(_Program, sizeof(char) * 512, &infoLen, &infoLog[0]);

	std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
}
