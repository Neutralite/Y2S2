#include "Texture.h"
#include <gmtk/gmtk.h>
#include "SOIL/SOIL.h"
#include "IO.h"
#include <vector>
#include <fstream>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS

std::string Texture::_TextureDirectory = "../assets/textures/";
std::string Texture::_TextureCubeDirectory = _TextureDirectory + "cubemap/";

float Texture::anisotropyAmount = 16.0f; 

GLenum filterModes[] =
{
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR
};

Texture::Texture(const std::string & file, bool mipmap)
{
	this->load(file, mipmap);
}

Texture::~Texture()
{
	this->unload();
}

bool Texture::load(const std::string & file, bool mipmap)
{
	textureName = file;
	this->filename = _TextureDirectory + file;

	unsigned char* textureData = SOIL_load_image((this->filename).c_str(),
		&this->sizeX, &this->sizeY, &this->channels, SOIL_LOAD_RGBA);

	if (this->sizeX == 0 || this->sizeY == 0 || this->channels == 0)
	{
		SAT_DEBUG_LOG_ERROR("TEXTURE BROKE: %s", this->filename.c_str());
		return false;
	}
	
	// If the texture is 2D, set it to be a 2D texture;
	_Target = GL_TEXTURE_2D;
	_InternalFormat = GL_RGBA8;	

	int levels = countMipMapLevels(mipmap);

	glGenTextures(1, &this->_Handle);
	this->bind();
	glTextureStorage2D(this->_Handle, levels, this->_InternalFormat, this->sizeX, this->sizeY);
	glTextureSubImage2D(this->_Handle, 0, // We are editing the first layer in memory (Regardless of mipmaps)
		0, 0, // No offset
		this->sizeX, this->sizeY, // the dimensions of our image loaded
		GL_RGBA, GL_UNSIGNED_BYTE, // Data format and type
		textureData); // Pointer to the texture data

	if (mipmap)
	{
		generateMipMaps();
	}

	glTextureParameteri(this->_Handle, GL_TEXTURE_MIN_FILTER, this->_Filter.min);
	glTextureParameteri(this->_Handle, GL_TEXTURE_MAG_FILTER, this->_Filter.mag);
	glTextureParameteri(this->_Handle, GL_TEXTURE_WRAP_S, this->_Wrap.x);
	glTextureParameteri(this->_Handle, GL_TEXTURE_WRAP_T, this->_Wrap.y);

	this->unbind();
	SOIL_free_image_data(textureData);
	return true;
}

bool Texture::load3D(const std::string & file, bool mipmap)
{
	this->filename = "../assets/LUTs/" + file;

	//unsigned char* textureData = SOIL_load_image((this->filename).c_str(),
	//	&this->sizeX, &this->sizeY, &this->channels, SOIL_LOAD_RGBA);
	std::vector<unsigned char> textureData;

	std::ifstream CUBE_FILE;
	CUBE_FILE.open("../assets/LUTs/" + file);
	std::string parseString;
	int iter = 0;
	int cubeSize = 0;
	while (std::getline(CUBE_FILE, parseString))
	{
		float r = 0;
		float g = 0;
		float b = 0;
		//std::cout << parseString << std::endl;
		if (iter == 5)
		{
			sscanf_s(parseString.c_str(), "LUT_3D_SIZE %f", &r);
			cubeSize = (int)r;
			//std::cout << r << std::endl;
		}
		else if (iter > 7)
		{
			sscanf_s(parseString.c_str(), "%f %f %f", &r, &g, &b);
			textureData.push_back((unsigned char)(r * 255.f));
			textureData.push_back((unsigned char)(g * 255.f));
			textureData.push_back((unsigned char)(b * 255.f));
			//std::cout << (int)textureData[(iter - 8) * 3] << ", " <<
			//	(int)textureData[(iter - 8) * 3 + 1] << ", " <<
			//	(int)textureData[(iter - 8) * 3 + 2] << ", " << std::endl;
		}
		++iter;
	}

	this->sizeX = cubeSize;
	this->sizeY = cubeSize;
	this->sizeZ = cubeSize;
	this->channels = 3;

	if (this->sizeX == 0 || this->sizeY == 0 || this->sizeZ == 0 || this->channels == 0)
	{
		SAT_DEBUG_LOG_ERROR("TEXTURE BROKE: %s", this->filename.c_str());
		return false;
	}

	// If the texture is 2D, set it to be a 2D texture;
	_Target = GL_TEXTURE_3D;
	_InternalFormat = GL_RGB8;

	int levels = countMipMapLevels(mipmap);

	glGenTextures(1, &this->_Handle);
	this->bind();
	glTextureStorage3D(this->_Handle, levels, this->_InternalFormat, this->sizeX, this->sizeY, this->sizeZ);
	glTextureSubImage3D(this->_Handle, 0, 0, // We are editing the first layer in memory (Regardless of mipmaps)
		0, 0, // No offset
		this->sizeX, this->sizeY, this->sizeZ, // the dimensions of our image loaded
		GL_RGB, GL_UNSIGNED_BYTE, // Data format and type
		&textureData[0]); // Pointer to the texture data

	if (mipmap)
	{
		generateMipMaps();
	}

	setWrapParameters(GL_CLAMP_TO_EDGE);

	_Wrap.x = GL_CLAMP_TO_EDGE;
	_Wrap.y = GL_CLAMP_TO_EDGE;
	_Wrap.z = GL_CLAMP_TO_EDGE;

	glTextureParameteri(this->_Handle, GL_TEXTURE_MIN_FILTER, this->_Filter.min);
	glTextureParameteri(this->_Handle, GL_TEXTURE_MAG_FILTER, this->_Filter.mag);
	glTextureParameteri(this->_Handle, GL_TEXTURE_WRAP_S, this->_Wrap.x);
	glTextureParameteri(this->_Handle, GL_TEXTURE_WRAP_T, this->_Wrap.y);
	glTextureParameteri(this->_Handle, GL_TEXTURE_WRAP_R, this->_Wrap.z);

	this->unbind();
	//SOIL_free_image_data(textureData);
	return true;
}

bool Texture::unload()
{
	if (this->_Handle)
	{
		glDeleteTextures(1, &this->_Handle);
		return true;
	}
	return false;
}

int Texture::countMipMapLevels(bool mipmap)
{
	int levels = 1;

	if (mipmap)
	{
		float largest = static_cast<float>(max(this->sizeX, this->sizeY));
		levels += static_cast<int>(std::floor(std::log2(largest)));
	}
	return levels;
}

void Texture::generateMipMaps()
{
	glGenerateTextureMipmap(this->_Handle);
	glTextureParameterf(this->_Handle, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyAmount);
}

void Texture::createTexture(int w, int h, GLenum target, GLenum filtering, GLenum edgeBehaviour, GLenum internalFormat, GLenum textureFormat, GLenum dataType, void * newDataPtr)
{
	sizeX = w;
	sizeY = h;
	_Filter.mag = filtering;
	_Wrap.x = edgeBehaviour;
	_Wrap.y = edgeBehaviour;
	_InternalFormat = internalFormat;
	_Target = target;

	GLenum error = 0;

	// Not necessary to enable GL_TEXTURE_* in modern context.
	//	glEnable(m_pTarget);
	//	error = glGetError();

	unload();

	glGenTextures(1, &_Handle);
	glBindTexture(target, _Handle);
	error = glGetError();

	glTexParameteri(_Target, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(_Target, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(_Target, GL_TEXTURE_WRAP_S, edgeBehaviour);
	glTexParameteri(_Target, GL_TEXTURE_WRAP_T, edgeBehaviour);
	error = glGetError();

	glTexImage2D(_Target, 0, internalFormat, w, h, 0, textureFormat, dataType, newDataPtr);
	error = glGetError();

	if (error != 0)
	{
		SAT_DEBUG_LOG_ERROR("[Texture.cpp : createTexture] Error when creating texture. ");
	}

	glBindTexture(_Target, 0);
}

void Texture::bind() const 
{
	glBindTexture(this->_Target, this->_Handle);
}

void Texture::bind(int textureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	this->bind();
}

void Texture::unbind() const
{
	glBindTexture(this->_Target, GL_NONE);
}

void Texture::unbind(int textureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	this->unbind();
}

GLuint Texture::getID()
{
	return _Handle;
}

std::string Texture::getName()
{
	return textureName;
}

void Texture::setName(std::string _NAME)
{
	textureName = _NAME;
}

void Texture::sendTexParameters()
{
	glTextureParameteri(this->_Handle, GL_TEXTURE_MIN_FILTER, this->_Filter.min);
	glTextureParameteri(this->_Handle, GL_TEXTURE_MAG_FILTER, this->_Filter.mag);
	glTextureParameteri(this->_Handle, GL_TEXTURE_WRAP_S, this->_Wrap.x);
	glTextureParameteri(this->_Handle, GL_TEXTURE_WRAP_T, this->_Wrap.y);
}

void Texture::setFilterParameters(GLenum mag, GLenum min)
{
	_Filter.mag = mag;
	_Filter.min = min;
}

void Texture::setWrapParameters(GLenum wrap)
{
	_Wrap.x = wrap;
	_Wrap.y = wrap;
	_Wrap.z = wrap;
}

TextureWrap::TextureWrap()
{
	x = GL_REPEAT;
	y = GL_REPEAT;
	z = GL_REPEAT;
}

