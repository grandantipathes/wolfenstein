#include "program.hpp"
#include <fstream>
#include <vector>
#include "events.hpp"


namespace wolf{

  program::program(const std::string& vt, const std::string& px):m_vertex_shader(0),
  m_fragment_shader(0), m_program(0)
  {
	  if (events::getInstance().get_context() == false)
		  return;

	  compile_shader(vt,GL_VERTEX_SHADER,m_vertex_shader);
    compile_shader(px,GL_FRAGMENT_SHADER,m_fragment_shader);

    m_program = glCreateProgram();

    glAttachShader(m_program, m_vertex_shader );
    glAttachShader(m_program, m_fragment_shader );
    glLinkProgram(m_program);

    // FREES MEMORY
    glDetachShader(m_program, m_vertex_shader);
    glDeleteShader(m_vertex_shader);

    glDetachShader(m_program, m_fragment_shader);
    glDeleteShader(m_fragment_shader);

  }

  program::~program()
  {
    
      if(glIsProgram(m_program)== GL_TRUE)
      {
          glDeleteProgram(m_program);
      }

  }
  void program::compile_shader(const std::string& path, GLenum shader_type, GLuint& shader)
  {
    std::ifstream file(path);
    std::string str;
    std::string content;

    while(std::getline(file,str))
    {
        content +=str;
        content.push_back('\n');
        //std::cout<<str<<std::endl;
    }

    //BLOODY NEEDS TO BE NULL TERMINATED
    content.push_back('\0');

    const char* cpath = content.c_str();

    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1,  &cpath, NULL);
    glCompileShader(shader);

    content.clear();

    GLint isCompiled = 0;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
	    std::cout<<path<<" has not compiled succesfuly\n";

	    GLint maxLength = 0;
	    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	    // The maxLength includes the NULL character
	    std::vector<GLchar> errorLog(maxLength);
	    glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

      glDeleteShader(shader);
    }
    else
    {
        std::cout<<path<<" has compiled succesfuly\n";
    }

  }

  const GLuint& program::get_program() const
  {
      return m_program;
  }
}
