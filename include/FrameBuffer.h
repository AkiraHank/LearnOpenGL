#pragma once

class FrameBuffer {
private:
  /* data */
  unsigned int m_renderID;
  unsigned int m_count;

public:
  FrameBuffer(/* args */);
  ~FrameBuffer();

  void bind() const;
  void unbind() const;
};
