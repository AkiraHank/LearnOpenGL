#include "IndicesBuffer.h"
#include <iostream>

IndicesBuffer::IndicesBuffer(const uint32_t* data, uint32_t count)
  :m_count(count)
{
  glGenBuffers(1, &m_renderID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
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