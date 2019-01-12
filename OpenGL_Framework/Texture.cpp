
#pragma comment(lib, "SOIL_ext.lib")
#include "Texture.h"
#include "SOIL/SOIL.h"

Texture::Texture(const std::string & file)
{
	this->load(file);
}

Texture::~Texture()
{
	this->unload();
}

bool Texture::load(const std::string & file)
{
	this->filename  = "Assets/Textures/" + file;

	unsigned char* textureData;
	textureData = SOIL_load_image((this->filename).c_str(), &this->sizeX, &this->sizeY, &this->channels, SOIL_LOAD_RGBA);

	if (this->sizeX == 0 || this->sizeY == 0 || this->channels == 0)
	{
		printf("TEXTURES BROKE: %s", (this->filename).c_str());
		return false;
	}
	
	//If the texture is 2D, set it to be a 2D texture
	_Target = GL_TEXTURE_2D;
	_InternalFormat = GL_RGBA8;

	glGenTextures(1, &this->_TexHandle);
	this->bind();
	glTexStorage2D(this->_Target, 1, this->_InternalFormat, this->sizeX, this->sizeY);

	glTexSubImage2D(this->_Target, 0 //Only one layer of memory
		, 0, 0 //no offsets
		, this->sizeX, this->sizeY //height and width of image(s)
		, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	glTexParameteri(this->_Target, GL_TEXTURE_MIN_FILTER, this->_FilterMin);
	glTexParameteri(this->_Target, GL_TEXTURE_MAG_FILTER, this->_FilterMag);
	glTexParameteri(this->_Target, GL_TEXTURE_WRAP_S, this->_WrapU);
	glTexParameteri(this->_Target, GL_TEXTURE_WRAP_T, this->_WrapV);


	this->unbind();
	SOIL_free_image_data(textureData);
	return true;
}

bool Texture::unload()
{
	if (_TexHandle != 0)
	{
		glDeleteTextures(1, &_TexHandle);
		return true;
	}
	return false;
}

void Texture::bind() const
{
	glBindTexture(this->_Target, this->_TexHandle);
}

void Texture::bind(int texSlot) const
{
	glActiveTexture(GL_TEXTURE0 + texSlot);
	this->bind();
}

void Texture::unbind() const
{
	glBindTexture(this->_Target, GL_NONE);
}

void Texture::unbind(int texSlot) const
{
	glActiveTexture(GL_TEXTURE0 + texSlot);
	this->unbind();
}
