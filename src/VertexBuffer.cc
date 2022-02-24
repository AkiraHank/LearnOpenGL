#include "VertexBuffer.h"
#include <iostream>

VertexBuffer::VertexBuffer(const void* data, uint32_t size)
{
  glGenBuffers(1, &m_renderID);
  glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
  glDeleteBuffers(1, &m_renderID);
}

void VertexBuffer::bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
}

void VertexBuffer::unbind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}