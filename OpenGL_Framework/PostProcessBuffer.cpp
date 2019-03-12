#include "PostProcessBuffer.h"

bool PostProcessBuffer::init(unsigned int width, unsigned int height)
{
	SAT_ASSERT(_IsInit == false, "POST PROCESS BUFFER HAS ALREADY BEEN INITIALIZED");
	if (!_IsInit)
	{
		for (int i = 0; i < 2; ++i)
		{
			m_pFramebuffers[i].addColorTarget(m_pFormat);
			m_pFramebuffers[i].init(width, height);
		}

		m_pReadBuffer = &m_pFramebuffers[0];
		m_pWriteBuffer = &m_pFramebuffers[1];

		_IsInit = true;
	}

	return _IsInit;
}

void PostProcessBuffer::setFormat(GLenum format)
{
	m_pFormat = format;
}

void PostProcessBuffer::clear()
{
	for (int i = 0; i < 2; ++i)
	{
		m_pFramebuffers[i].clear();
	}
}

void PostProcessBuffer::reshape(unsigned int width, unsigned int height)
{
	for (int i = 0; i < 2; ++i)
	{
		m_pFramebuffers[i].reshape(width, height);

		setOpenGLName(GL_TEXTURE,
			m_pFramebuffers->_Color._Tex[0].getID(),
			"PostProcessing FBO #" + std::to_string(i));
	}
}

void PostProcessBuffer::drawToPost()
{
	SAT_ASSERT(_IsInit == true, "UNINITIALIZED POST PROC BUFFER BEING DRAWN! - TYPE 1");
	m_pWriteBuffer->renderToFSQ();
	swap();
}

void PostProcessBuffer::draw()
{
	SAT_ASSERT(_IsInit == true, "UNINITIALIZED POST PROC BUFFER BEING DRAWN! - TYPE 2");
	m_pReadBuffer->bindColorAsTexture(0, 0);
	m_pWriteBuffer->renderToFSQ();
	m_pReadBuffer->unbindTexture(0);
	swap();
}

void PostProcessBuffer::drawToScreen()
{
	SAT_ASSERT(_IsInit == true, "UNINITIALIZED POST PROC BUFFER BEING DRAWN! - TYPE 3");
	m_pWriteBuffer->unbind();
	m_pReadBuffer->bindColorAsTexture(0, 0);
	Framebuffer::drawFSQ();
	m_pReadBuffer->unbindTexture(0);
	clear();
}

void PostProcessBuffer::drawToFB(Framebuffer * FB)
{
	SAT_ASSERT(_IsInit == true, "UNINITIALIZED POST PROC BUFFER BEING DRAWN! - TYPE 3");
	m_pWriteBuffer->unbind();
	m_pReadBuffer->bindColorAsTexture(0, 0);
	FB->renderToFSQ();
	m_pReadBuffer->unbindTexture(0);
}

void PostProcessBuffer::swap()
{
	std::swap(m_pReadBuffer, m_pWriteBuffer);
}