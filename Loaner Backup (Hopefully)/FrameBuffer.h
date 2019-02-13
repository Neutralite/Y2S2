#pragma once
#include "GL\glew.h"

class FrameBuffer
{
public:
	FrameBuffer() = delete;
	FrameBuffer(unsigned numColorAttachments);
	~FrameBuffer();

	void InitDepthTexture(unsigned width, unsigned height);
	void InitColorTexture(unsigned index, unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap);
	bool CheckFBO();

	void Unload();

	//GET RID OF ALL TEXTURES.
	void Clear();

	void Bind();
	void Unbind();

	void MovetoBackBuffer(int windowWidth, int windowHeight);
	unsigned int getNumColorAttachments();

	GLuint GetDepthHandle() const;
	GLuint GetColorHandle(unsigned int index) const;

	void resize(unsigned int width, unsigned int height);

private:
	GLuint _FBO = GL_NONE;
	GLuint _DepthAttachment = GL_NONE;
	GLuint *_ColorAttachments = nullptr;
	GLenum *_Bufs = nullptr;

	GLint* _WrapType = nullptr;
	GLint* _FilterType = nullptr;
	GLint* _InternalFormat = nullptr;

	bool _IsInit = false;
	bool _DepthInit = false;

	unsigned int _NumColorAttachments = 0;
};