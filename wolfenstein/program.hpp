#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>

namespace wolf{

class program{

public:
  program(const std::string& vt, const std::string& px);
  ~program();
  program(const program&) = delete;
  program& operator=(const program&)= delete;

  void compile_shader(const std::string& path, GLenum shader_type, GLuint& shader);
  const GLuint & get_program() const;


private:
  GLuint m_vertex_shader, m_fragment_shader, m_program;
};


}
