#include "VertexBuffer.h"
#include <iostream>
#include "glad/glad.h" // 包含glad来获取所有的必须OpenGL头文件

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
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