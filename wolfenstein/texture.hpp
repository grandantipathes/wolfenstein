#pragma once
#include "program.hpp"

namespace wolf{

class texture{
public:
  texture();
  ~texture();

  //Compiler will raise exception if trying to copy texture
  texture(const texture&)=delete;
  texture& operator=(const texture&)=delete;
  //move constructor
  texture(texture&& target);
  texture& operator=(texture &&target);

  void gl_register(const program& in_program, const std::string& path, const char* name, GLuint number);
  bool is_registered() const;

  void draw() const;

private:

  void release();
  GLuint m_texture_id, m_number;
  GLint m_texture_loc;
  const char* m_name;
  bool m_registered;
};
}
