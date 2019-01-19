#include "glfw.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "events.hpp"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);
}

static void glfw_key_callback(GLFWwindow* window,int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        wolf::events::getInstance().broadcast("closewindow");
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        wolf::events::getInstance().broadcast("backward");
    }

    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        wolf::events::getInstance().broadcast("forward");
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        wolf::events::getInstance().broadcast("left");
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        wolf::events::getInstance().broadcast("right");
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        wolf::events::getInstance().broadcast("speedup");
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        wolf::events::getInstance().print();
        wolf::events::getInstance().broadcast("print");
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        wolf::events::getInstance().broadcast("delete");
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        wolf::events::getInstance().broadcast("onoff");
    }


}

namespace wolf {

glfw::glfw():should_close(0),m_initialized(0),m_window(nullptr)
{
    std::function<void()> mclose = [this](){this->close_window();};
		events::getInstance().register_event("dummy","closewindow", mclose);
}

glfw::~glfw()
{}

void glfw::init_glfw()
{

    if(m_initialized)
        return;


    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(640, 480, "Wolfenstein", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    m_initialized = 1;

    glfwSetKeyCallback(m_window, glfw_key_callback);

    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    //VSYNC
    glfwSwapInterval(1);
    glfwSetInputMode(m_window,GLFW_STICKY_KEYS,1);

	wolf::events::getInstance().set_context(true);
}

GLFWwindow* const wolf::glfw::get_window() const
{
    return m_window;
}

void glfw::end()
{
     glfwDestroyWindow(m_window);
     glfwTerminate();
     exit(EXIT_SUCCESS);
}

void glfw::close_window()
{
     glfwSetWindowShouldClose(m_window, GLFW_TRUE);
     should_close = 1;
}
}
