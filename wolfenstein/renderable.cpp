#include <assert.h>
#include <fstream>
#include <iostream>
#include "renderable.hpp"
#include "program.hpp"
#include "events.hpp"

namespace wolf{

renderable::renderable()
{
    std::cout<<"RENDERABLE CONSTRUCTOR CALLED\n";
}
void renderable::render(const polymesh& in_polymesh, const program& in_program, const std::vector<texture>& in_texture) const
{

    glUseProgram(in_program.get_program());

    //in_polymesh.print();

    if(in_polymesh.is_registered())
      in_polymesh.draw();

    auto it = in_texture.begin();
	  while (it != in_texture.end())
	   {
		     if (it->is_registered())
			      it->draw();
		          ++it;
	           }


    glDrawElements(GL_TRIANGLES,in_polymesh.size(),GL_UNSIGNED_INT,(void*)0);

}
}
