#include "texture.hpp"
#include "events.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace wolf{

  texture::texture():m_texture_id(0),m_number(0),
  m_texture_loc(0),m_name(""),m_registered(false)
  {
     std::cout<<"TEXTURE CONSTRUCTOR CALLED\n";
  }
  void texture::gl_register(const program& in_program, const std::string& path, const char* name, GLuint number)
  {
	if (events::getInstance().get_context() == false)
		  return;
	if(m_registered == true)
      return;

    m_number = number;
    m_name =  name;

    int width, height, num_channels;
    unsigned char *texture_data = stbi_load(path.c_str(),&width,&height,&num_channels,0);

    glGenTextures(1,&m_texture_id);
    glBindTexture(GL_TEXTURE_2D,m_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    assert(in_program.get_program()!=0);
    glUseProgram(in_program.get_program());
    //Needs above to work
    m_texture_loc = glGetUniformLocation(in_program.get_program(), name);

    std::cout<<"texture location for: "<< path <<" is: "<< m_texture_loc <<std::endl;
    //modifies the value of uniform variable m_texture_loc
    glUniform1i(m_texture_loc,number);

    if(texture_data)
    {

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else
    {
        std::cout<<"texture loading failed: "<<path<<std::endl;
    }

    stbi_image_free(texture_data);

    m_registered = true;

  }


  texture::~texture()
  {
    release();
  }


  void texture::release()
  {
     std::cout<<"texture destructor called\n";
     glDeleteTextures(1,&m_texture_id);
     m_texture_id = 0;

  }


  void texture::draw() const
  {
    switch(m_number)
    {
      case(0):
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_texture_id);
        break;
      case(1):
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,m_texture_id);
        break;
      case(2):
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,m_texture_id);
        break;
      case(3):
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,m_texture_id);
        break;
      default:
        std::cout<<"texture number out of range\n";
        return;
    }

  }

  bool texture::is_registered() const
  {
      return m_registered;
  }

  texture::texture(texture&& target):
  m_texture_id(target.m_texture_id),m_number(target.m_number),
  m_texture_loc(target.m_texture_loc),m_name(target.m_name),
  m_registered(target.m_registered)
  {
    std::cout<<"texture move constructor called\n";
    //FREE MEM + initialize to 0

    target.m_texture_id = 0;
    target.m_number = 0;
    target.m_texture_loc = 0;
    target.m_name = "";
    target.m_registered = false;
  }

  texture& texture::operator=(texture &&target)
  {
      std::cout<<"texture move operator= called\n";

      if(this!=&target)
      {
        release();
        std::swap(m_texture_id,target.m_texture_id);
        std::swap(m_number,target.m_number);
        std::swap(m_texture_loc,target.m_texture_loc);
        std::swap(m_name,target.m_name);
        std::swap(m_registered,target.m_registered);

      }
      return *this;
  }
}
