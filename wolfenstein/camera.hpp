#pragma once
#include <glm/glm.hpp>
//GLAD needs to always be included prior glfw3.h to avoid GL error
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "program.hpp"

namespace wolf{
class camera
{
    public:
    camera();
    ~camera();
    camera(const camera&) = delete;
    camera& operator=(const camera& cam)= delete;

    void gl_register(const program& in_program);
    void update(GLFWwindow* const window, const int* const width, const int* const height);
    bool is_registered() const;

    void move_forward();
    void move_backward();
    void move_left();
    void move_right();
    void speed_up();

    const glm::mat4& get_mvp() const;
    const glm::mat4& get_camera() const;
    const glm::mat4& get_proj() const;
    const glm::mat4& get_world() const;
    const glm::mat3& get_normal() const;

    void draw() const;

    private:
    glm::mat4 m_world, m_camera, m_proj, m_mvp;
    glm::vec3 m_campos, m_up, m_target, m_direction, m_right;
    float m_fov, m_near, m_far;
    glm::mat3 m_normal_matrix;
    float m_pitch, m_yaw, m_speed, m_cursor_speed;
    double m_render_time;
    GLint m_mvp_loc,m_camera_loc, m_world_loc, m_normal_matrix_loc;
    bool m_registered;
};
}
