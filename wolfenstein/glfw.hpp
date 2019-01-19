#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace wolf{
class glfw {
public:
	glfw();
	~glfw();
	glfw(glfw const&) = delete;
	void operator=(glfw const&)=delete;
    void init_glfw();
    GLFWwindow* const get_window() const;
    void end();
    void close_window();
    bool should_close;

private:
	bool m_initialized;
	GLFWwindow* m_window;
};
}
