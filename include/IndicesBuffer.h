#pragma once

class IndicesBuffer {
private:
  unsigned int m_renderID;
  unsigned int m_count;
public:
  IndicesBuffer(const unsigned int* data, unsigned int count);
  ~IndicesBuffer();

  void bind() const;
  void unbind() const;
};