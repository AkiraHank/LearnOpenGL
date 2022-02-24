#pragma once

#include "glad/glad.h" // 包含glad来获取所有的必须OpenGL头文件

class IndicesBuffer {
private:
  uint32_t m_renderID;
  uint32_t m_count;
public:
  IndicesBuffer(const uint32_t* data, uint32_t count);
  ~IndicesBuffer();

  void bind() const;
  void unbind() const;
};