#include "IndicesBuffer.h"
#include <iostream>
#include "glad/glad.h" // 包含glad来获取所有的必须OpenGL头文件

IndicesBuffer::IndicesBuffer(const unsigned int* data, unsigned int count)
  :m_count(count)
{
  glGenBuffers(1, &m_renderID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndicesBuffer::~IndicesBuffer()
{
  glDeleteBuffers(1, &m_renderID);
}

void IndicesBuffer::bind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID);
}

void IndicesBuffer::unbind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}