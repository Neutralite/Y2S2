#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(unsigned numColorAttachments)
{
	_NumColorAttachments = numColorAttachments;

	glGenFramebuffers(1, &_FBO);

	_ColorAttachments = new GLuint[_NumColorAttachments];
	_WrapType = new GLint[_NumColorAttachments];
	_FilterType = new GLint[_NumColorAttachments];
	_InternalFormat = new GLint[_NumColorAttachments];

	//_Bufs is required as a parameter for glDrawBuffers

	_Bufs = new GLenum[_NumColorAttachments];
	for (int i = 0; i < _NumColorAttachments; i++)
	{
		_Bufs[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	_IsInit = true;
}

FrameBuffer::~FrameBuffer()
{
	Unload();
}

void FrameBuffer::InitDepthTexture(unsigned width, unsigned height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	//Create depth Texture
	glGenTextures(1, &_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, _DepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Bind to buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _DepthAttachment, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

	_DepthInit = true;
}

void FrameBuffer::InitColorTexture(unsigned index, unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	//Create depth Texture
	glGenTextures(1, &_ColorAttachments[index]);
	glBindTexture(GL_TEXTURE_2D, _ColorAttachments[index]);
	glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	//Bind to buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, _ColorAttachments[index], 0);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

	_WrapType[index] = wrap;
	_FilterType[index] = filter;
	_InternalFormat[index] = internalFormat;
}

bool FrameBuffer::CheckFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Unload();
		return false;
	}
	return true;
}

void FrameBuffer::Unload()
{
	if (_Bufs != nullptr)
	{
		delete[] _Bufs;
		_Bufs = nullptr;
	}

	if (_ColorAttachments != nullptr)
	{
		for (int i = 0; i < _NumColorAttachments; i++)
		{
			glDeleteTextures(1, &_ColorAttachments[i]);
		}

		delete[] _WrapType;
		_WrapType = nullptr;

		delete[] _FilterType;
		_FilterType = nullptr;

		delete[] _InternalFormat;
		_InternalFormat = nullptr;

		delete[] _ColorAttachments;
		_ColorAttachments = nullptr;
	}

	if (_DepthAttachment != GL_NONE)
	{
		glDeleteTextures(1, &_DepthAttachment);
		_DepthAttachment = GL_NONE;
	}

	glDeleteFramebuffers(1, &_FBO);
	_IsInit = false;
	//_NumColorAttachments = 0;
}

void FrameBuffer::Clear()
{
	GLbitfield temp = 0;

	if (_DepthAttachment != GL_NONE)
	{
		temp = temp | GL_DEPTH_BUFFER_BIT;
	}

	if (_ColorAttachments != nullptr)
	{
		temp = temp | GL_COLOR_BUFFER_BIT;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glClear(temp);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glDrawBuffers(_NumColorAttachments, _Bufs);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::MovetoBackBuffer(int windowWidth, int windowHeight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

unsigned int FrameBuffer::getNumColorAttachments()
{
	return _NumColorAttachments;
}

GLuint FrameBuffer::GetDepthHandle() const
{
	return _DepthAttachment;
}

GLuint FrameBuffer::GetColorHandle(unsigned int index) const
{
	return _ColorAttachments[index];
}

void FrameBuffer::resize(unsigned int width, unsigned int height)
{
	if (_IsInit)
	{
		GLint* _DOUBLE = new GLint[_NumColorAttachments];
		GLint* _DOUBLE2 = new GLint[_NumColorAttachments];
		GLint* _DOUBLE3 = new GLint[_NumColorAttachments];

		for (int i = 0; i < _NumColorAttachments; i++)
		{
			_DOUBLE[i] = _WrapType[i];
			_DOUBLE2[i] = _FilterType[i];
			_DOUBLE3[i] = _InternalFormat[i];
		}

		Unload();
		glGenFramebuffers(1, &_FBO);

		_ColorAttachments = new GLuint[_NumColorAttachments];
		_WrapType = new GLint[_NumColorAttachments];
		_FilterType = new GLint[_NumColorAttachments];
		_InternalFormat = new GLint[_NumColorAttachments];

		//_Bufs is required as a parameter for glDrawBuffers

		_Bufs = new GLenum[_NumColorAttachments];
		for (int i = 0; i < _NumColorAttachments; i++)
		{
			_Bufs[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		InitDepthTexture(width, height);
		for (int i = 0; i < _NumColorAttachments; i++)
			InitColorTexture(i, width, height, _DOUBLE3[i], _DOUBLE2[i], _DOUBLE[i]);

		delete[] _DOUBLE;
		delete[] _DOUBLE2;
		delete[] _DOUBLE3;

		_IsInit = true;
	}
}
