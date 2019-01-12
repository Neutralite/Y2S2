#pragma once
#include "GL/glew.h"
#include <string>

class Texture
{
public:
	Texture() = default;
	Texture(const std::string &file);
	~Texture();

	bool load(const std::string &file);
	bool unload();
	
	void bind() const;
	void bind(int texSlot) const;
	void unbind()const;
	void unbind(int texSlot)const;

private:
	GLenum _InternalFormat;
	std::string filename;
	GLuint _TexHandle;
	GLuint _Target;
	GLenum _FilterMag = GL_LINEAR, 
		_FilterMin = GL_LINEAR, 
		_WrapU = GL_MIRRORED_REPEAT, 
		_WrapV = GL_MIRRORED_REPEAT;

	int sizeX, sizeY;
	int channels;
};