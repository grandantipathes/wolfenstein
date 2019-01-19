#pragma once
#include <glad/glad.h>
#include "polymesh.hpp"
#include <string>
#include <iostream>
#include "program.hpp"
#include "texture.hpp"
#include "camera.hpp"

namespace wolf{
class renderable
{
    public:
    renderable();
    void render(const polymesh& in_polymesh,
                const program& in_program,
                const std::vector<texture>& in_texture
              )const;

    private:
    renderable(const renderable&){std::cout<<"renderable copy constructor called\n";}
};
}
