#include "glfw.hpp"
#include <glm/glm.hpp>
#include "renderable.hpp"
#include "camera.hpp"
#include "debuggl.hpp"
#include "scene.hpp"

int main(void)
{

    wolf::glfw game;
    game.init_glfw();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(wolf::debuggl::gl_err_callback, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                      GL_DONT_CARE, 0, NULL, GL_TRUE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

    std::string vt_path = "./shaders/simple_vt.glsl";
    std::string px_path = "./shaders/simple_px.glsl";
    wolf::program program(vt_path,px_path);

    wolf::polymesh polymesh;
    polymesh.create_cube();

	//NEW SHIT

	std::vector<wolf::polymesh*> myvec;
	std::string abc_path = "./abc/geo.abc";
	wolf::scene abc;
	abc.test(abc_path);
	//Sleep(500000);

	abc.load_abc(abc_path, false, myvec);

	myvec[0]->to_point();
	myvec[0]->gl_register(program);
	//myvec[0]->print();
	
	//std::cout << myvec[0]->vertices().size() << std::endl;
	//std::cout << myvec[0]->normals().size() << std::endl;
	//std::cout << myvec[0]->uvs().size() << std::endl;
	//std::cout << myvec[0]->indices().size() << std::endl;


    //polymesh.gl_register(program);

    //std::vector<wolf::polymesh> meshes;
    //meshes.push_back(std::move(polymesh));

    wolf::texture albedo,albedo1;
    albedo.gl_register(program,"./textures/crate.jpg","texture1",0);
    albedo1.gl_register(program,"./textures/crate2.jpg","texture2",1);


	std::vector<wolf::texture> bundle;
    //use of move constructor
    bundle.push_back(std::move(albedo));
    bundle.push_back(std::move(albedo1));

    wolf::camera camera;
    camera.gl_register(program);

    wolf::renderable renderable;

	

    while (!game.should_close)
    {

        int width, height;

        glfwGetFramebufferSize(game.get_window(), &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.update(game.get_window(),&width,&height);

        renderable.render(  *myvec[0],
                            program,
                            bundle
                            );

        glfwSwapBuffers(game.get_window());
        glfwPollEvents();

    }

    game.end();
}
