#pragma once

#include "glad/glad.h" // 包含glad来获取所有的必须OpenGL头文件

class VertexBuffer {
private:
  uint32_t m_renderID;

public:
  VertexBuffer(const void* data, uint32_t size);
  ~VertexBuffer();

  void bind() const;
  void unbind() const;
};