#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "program.hpp"

namespace wolf{
typedef struct point_3d {
    float vertex[3];
    float normal[3];
    float uv[2];
} point_3d;


class polymesh
{
    public:

    polymesh();
    ~polymesh();
	//Compiler will raise an exception if copy constructor is called
	polymesh(const polymesh&) = delete;
	polymesh& operator=(const polymesh&) = delete;
	//move constructor
	polymesh(polymesh&& target);
	polymesh& operator=(polymesh &&target);

    void create_cube();
    void to_point();
    void gl_register(const program& in_program);
	void register_to_events();
    

    

	void draw() const;
    void on_off();
    void free_mem();

    const std::vector<glm::vec3>& vertices() const;
    const std::vector<unsigned int>& indices() const;
    const std::vector<glm::vec3>& normals() const;
    const std::vector<glm::vec2>& uvs() const;
    const std::vector<point_3d>& aspoint() const;
    const unsigned int& size() const;
    bool is_registered() const;
    
    void print() const;


	void set_position(const std::vector<glm::vec3>& in_points);
	void set_indices(const std::vector<glm::vec3>& in_points);
	void set_normals(const std::vector<glm::vec3>& in_normals);
	void set_uvs(const std::vector<glm::vec2>& in_uvs);

	std::vector<glm::vec3> unindexed(const std::vector<glm::vec3>& invertices, const std::vector<unsigned int>& inindices);
	std::vector<glm::vec2> unindexed(const std::vector<glm::vec2>& invertices, const std::vector<unsigned int>& inindices);
	
	void swap_winding_order(std::vector<unsigned int> & indices,
		const std::vector<unsigned int>& counts);
	
	bool is_valid() const;
	bool is_valid(const std::vector<unsigned int>& vertIndices,
		const std::vector<unsigned int>& faceCounts);

    private:

    void release();
    std::vector<glm::vec3> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_uvs;
    std::vector<point_3d> m_point_data;
    bool m_registered;

    GLuint m_vao, m_vbo, m_ibo;
    GLint m_pos_loc, m_normal_loc, m_uv_loc;
    unsigned int m_indices_size;
};
}
