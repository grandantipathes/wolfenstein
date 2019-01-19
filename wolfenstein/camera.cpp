#include "camera.hpp"
#include "events.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <iostream>
#include <sstream>



namespace wolf {

camera::camera():m_world(1.0f), m_camera(1.0f), m_proj(1.0f), m_mvp(1.0f), m_campos(0.0f,0.0f,5.0f), m_up(0.0f,1.0f,0.0f),
m_target(0.0f), m_direction(0.0f,0.0f,0.0f), m_right(1.0f,0.0f,0.0f), m_fov(55.0f), m_near(0.1f), m_far(100.0f),
m_normal_matrix(1.0f), m_pitch(3.14f), m_yaw(0.0f), m_speed(30.0f), m_cursor_speed(0.001f), m_render_time(0.0),
m_mvp_loc(0),m_camera_loc(0), m_world_loc(0), m_normal_matrix_loc(0),m_registered(false)
{

    std::cout<<"MATRICES CONSTRUCTOR CALLED\n";

    const void * address = static_cast<const void*>(this);
    std::stringstream ss;
    ss << address;
    std::string name = ss.str();


    std::function<void()> mfor = [this](){this->move_forward();};
		events::getInstance().register_event(name,"forward", mfor);

	std::function<void()> mback = [this](){this->move_backward();};
		events::getInstance().register_event(name,"backward", mback);

    std::function<void()> mleft = [this](){this->move_left();};
		events::getInstance().register_event(name,"left", mleft);

	std::function<void()> mright = [this](){this->move_right();};
		events::getInstance().register_event(name,"right", mright);

    std::function<void()> mspeed = [this](){this->speed_up();};
		events::getInstance().register_event(name,"speedup", mspeed);

}

camera::~camera()
{

}

void camera::gl_register(const program& in_program)
{
	if (events::getInstance().get_context() == false)
		return;
	if(m_registered == true)
      return;

    //make sure there is an active program to bind to
    assert(in_program.get_program()!=0);
    glUseProgram(in_program.get_program());

    m_mvp_loc = glGetUniformLocation(in_program.get_program(), "mvp");
    m_camera_loc = glGetUniformLocation(in_program.get_program(), "camera");
    m_world_loc = glGetUniformLocation(in_program.get_program(), "world");
    m_normal_matrix_loc = glGetUniformLocation(in_program.get_program(), "nmat");

    m_registered = true;
}

void camera::update(GLFWwindow* const window, const int* const width, const int* const height)
{


    static double previous_time = glfwGetTime();
    double current_time = glfwGetTime();
    //How much time between 2 frames
    m_render_time = (current_time - previous_time);

    //RENDER TIMES
    //std::cout<<double(1/m_render_time)<<std::endl;

    double xpos,ypos;
    glfwGetCursorPos(window,&xpos,&ypos);

    //reset cursor position to the center of the window for the next frame
    glfwSetCursorPos(window,(*width)/2 ,(*height)/2);

    m_yaw += m_cursor_speed * float((*height)/2 - ypos);
    m_pitch += m_cursor_speed * float((*width)/2 - xpos);

    m_direction[0] = cos(m_yaw) * sin(m_pitch);
    m_direction[1] = sin(m_yaw);
    m_direction[2] = cos(m_yaw) * cos(m_pitch);


    m_right[0] = sin(m_pitch - 3.14f/2.0f);
    m_right[1] = 0.0f;
    m_right[2] = cos(m_pitch - 3.14f/2.0f);

    m_up = glm::cross(m_right,m_direction);

    m_proj = glm::perspective(glm::radians(m_fov),
                                       static_cast<float>(*width) / static_cast<float>(*height),
                                       m_near,
                                       m_far);

    m_camera = glm::lookAt(
                           m_campos,
                           m_campos+m_direction,
                           m_up);

    m_normal_matrix = glm::inverseTranspose(glm::mat3(m_camera*m_world));

    m_mvp = m_proj * m_camera * m_world;

    previous_time = current_time;


    draw();

}

void camera::move_forward()
{
    m_campos += glm::vec3(m_direction[0],0.0f, m_direction[2]) * float(m_speed * m_render_time);
    //std::cout<<m_campos[0]<<"   "<<m_campos[1]<<"   "<<m_campos[2]<<std::endl;
}

void camera::move_backward()
{
    m_campos -= glm::vec3(m_direction[0],0.0f, m_direction[2]) * float(m_speed * m_render_time);
    //std::cout<<m_campos[0]<<"   "<<m_campos[1]<<"   "<<m_campos[2]<<std::endl;
}

void camera::move_left()
{
    m_campos -= m_right * float(m_speed * m_render_time);
    //std::cout<<m_campos[0]<<"   "<<m_campos[1]<<"   "<<m_campos[2]<<std::endl;

}

void camera::move_right()
{
    m_campos += m_right * float(m_speed * m_render_time);
    //std::cout<<m_campos[0]<<"   "<<m_campos[1]<<"   "<<m_campos[2]<<std::endl;

}

void camera::speed_up()
{
    m_speed += 1.0f;
}


const glm::mat4& camera::get_world() const
{
    return m_world;
}

const glm::mat4& camera::get_camera() const
{
    return m_camera;
}

const glm::mat4& camera::get_proj() const
{
    return m_proj;
}

const glm::mat4& camera::get_mvp() const
{
    return m_mvp;
}

const glm::mat3& camera::get_normal() const
{
    return m_normal_matrix;
}

void camera::draw() const
{
    glUniformMatrix4fv(m_mvp_loc, 1, GL_FALSE, (const GLfloat*)&m_mvp[0][0]);
    glUniformMatrix4fv(m_camera_loc, 1, GL_FALSE, (const GLfloat*)&m_camera[0][0]);
    glUniformMatrix4fv(m_world_loc, 1, GL_FALSE, (const GLfloat*)&m_world[0][0]);
    glUniformMatrix3fv(m_normal_matrix_loc, 1, GL_FALSE, (const GLfloat*)&m_normal_matrix[0][0]);
}

bool camera::is_registered() const
{
    return m_registered;
}
}
