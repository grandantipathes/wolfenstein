#include "polymesh.hpp"
#include <iostream>
#include "events.hpp"
#include <sstream>
#include <algorithm>


namespace wolf{

polymesh::polymesh():m_vertices(0),m_indices(0),
m_normals(0),m_uvs(0),m_point_data(0),
m_registered(false),m_vao(0),m_vbo(0), m_ibo(0),m_pos_loc(0),
m_normal_loc(0), m_uv_loc(0), m_indices_size(0)
{
    std::cout<<"POLYMESH CONSTRUCTOR CALLED\n";
    register_to_events();
}

void polymesh::register_to_events()
{

  const void * address = static_cast<const void*>(this);
  std::stringstream ss;
  ss << address;
  std::string name = ss.str();


  std::function<void()> mdelete = [this](){this->free_mem();};
  events::getInstance().register_event(name,"delete", mdelete);

  std::function<void()> monoff = [this](){this->on_off();};
  events::getInstance().register_event(name,"onoff", monoff);

  std::function<void()> mprint = [this](){this->print();};
  events::getInstance().register_event(name,"print", mprint);
}

void polymesh::create_cube()
{
   m_indices = { 	0, 1, 2, 0, 2, 3, //front
              		4, 7, 6, 6, 5, 4, //right
                    10, 9, 8, 8, 11, 10, //back
                    12, 13, 14, 14, 15, 12, //left
                    18, 17, 16, 16, 19, 18, //upper
                    22, 23, 20, 20, 21, 22}; //bottom

  //front
  m_vertices.push_back(glm::vec3(-1.0, -1.0, 1.0));
  m_vertices.push_back(glm::vec3( 1.0, -1.0, 1.0));
  m_vertices.push_back(glm::vec3( 1.0, 1.0, 1.0));
  m_vertices.push_back(glm::vec3(-1.0, 1.0, 1.0));


  //right
  m_vertices.push_back(glm::vec3(1.0, 1.0, 1.0));
  m_vertices.push_back(glm::vec3(1.0, 1.0, -1.0));
  m_vertices.push_back(glm::vec3(1.0, -1.0, -1.0));
  m_vertices.push_back(glm::vec3(1.0, -1.0, 1.0));

  //back
  m_vertices.push_back(glm::vec3(-1.0, -1.0, -1.0));
  m_vertices.push_back(glm::vec3( 1.0, -1.0, -1.0));
  m_vertices.push_back(glm::vec3( 1.0, 1.0, -1.0));
  m_vertices.push_back(glm::vec3(-1.0, 1.0, -1.0));

  //left
  m_vertices.push_back(glm::vec3(-1.0, -1.0, -1.0));
  m_vertices.push_back(glm::vec3(-1.0, -1.0, 1.0));
  m_vertices.push_back(glm::vec3(-1.0, 1.0, 1.0));
  m_vertices.push_back(glm::vec3(-1.0, 1.0, -1.0));

  //upper
  m_vertices.push_back(glm::vec3( 1.0, 1.0, 1.0));
  m_vertices.push_back(glm::vec3(-1.0, 1.0, 1.0));
  m_vertices.push_back(glm::vec3(-1.0, 1.0, -1.0));
  m_vertices.push_back(glm::vec3( 1.0, 1.0, -1.0));

  //bottom
  m_vertices.push_back(glm::vec3(-1.0, -1.0, -1.0));
  m_vertices.push_back(glm::vec3( 1.0, -1.0, -1.0));
  m_vertices.push_back(glm::vec3( 1.0, -1.0, 1.0));
  m_vertices.push_back(glm::vec3(-1.0, -1.0, 1.0));

  //Front Normal
  m_normals.push_back(glm::vec3(0.0,0.0,1.0));
  m_normals.push_back(glm::vec3(0.0,0.0,1.0));
  m_normals.push_back(glm::vec3(0.0,0.0,1.0));
  m_normals.push_back(glm::vec3(0.0,0.0,1.0));

 //Right Normal
  m_normals.push_back(glm::vec3(1.0,0.0,0.0));
  m_normals.push_back(glm::vec3(1.0,0.0,0.0));
  m_normals.push_back(glm::vec3(1.0,0.0,0.0));
  m_normals.push_back(glm::vec3(1.0,0.0,0.0));

   //back Normal
  m_normals.push_back(glm::vec3(0.0,0.0,-1.0));
  m_normals.push_back(glm::vec3(0.0,0.0,-1.0));
  m_normals.push_back(glm::vec3(0.0,0.0,-1.0));
  m_normals.push_back(glm::vec3(0.0,0.0,-1.0));

  //Left Normal
  m_normals.push_back(glm::vec3(-1.0,0.0,0.0));
  m_normals.push_back(glm::vec3(-1.0,0.0,0.0));
  m_normals.push_back(glm::vec3(-1.0,0.0,0.0));
  m_normals.push_back(glm::vec3(-1.0,0.0,0.0));

  //Upper Normal
  m_normals.push_back(glm::vec3(0.0,1.0,0.0));
  m_normals.push_back(glm::vec3(0.0,1.0,0.0));
  m_normals.push_back(glm::vec3(0.0,1.0,0.0));
  m_normals.push_back(glm::vec3(0.0,1.0,0.0));

  //Bottom Normal

  m_normals.push_back(glm::vec3(0.0,-1.0,0.0));
  m_normals.push_back(glm::vec3(0.0,-1.0,0.0));
  m_normals.push_back(glm::vec3(0.0,-1.0,0.0));
  m_normals.push_back(glm::vec3(0.0,-1.0,0.0));

  m_uvs.push_back(glm::vec2(0.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,1.0));
  m_uvs.push_back(glm::vec2(0.0,1.0));

  m_uvs.push_back(glm::vec2(0.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,1.0));
  m_uvs.push_back(glm::vec2(0.0,1.0));

  m_uvs.push_back(glm::vec2(0.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,1.0));
  m_uvs.push_back(glm::vec2(0.0,1.0));

  m_uvs.push_back(glm::vec2(0.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,1.0));
  m_uvs.push_back(glm::vec2(0.0,1.0));

  m_uvs.push_back(glm::vec2(0.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,1.0));
  m_uvs.push_back(glm::vec2(0.0,1.0));

  m_uvs.push_back(glm::vec2(0.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,0.0));
  m_uvs.push_back(glm::vec2(1.0,1.0));
  m_uvs.push_back(glm::vec2(0.0,1.0));


  m_indices_size = m_indices.size();

  to_point();

}

const std::vector<glm::vec3>& polymesh::vertices() const
{
    return m_vertices;
}

const std::vector<unsigned int>& polymesh::indices() const
{
    return m_indices;
}

const std::vector<glm::vec3>& polymesh::normals() const
{
    return m_normals;
}

const std::vector<glm::vec2>& polymesh::uvs() const
{
	return m_uvs;
}

bool polymesh::is_valid() const
{
	return((m_normals.size()==m_vertices.size()) && ((m_normals.size() == m_uvs.size())) );
}


void polymesh::to_point()
{
	assert(is_valid());
	for(unsigned int i=0; i<m_vertices.size();i++)
	{
		m_point_data.push_back(point_3d());

		m_point_data[i].vertex[0] = float(m_vertices[i][0]);
		m_point_data[i].vertex[1] = float(m_vertices[i][1]);
		m_point_data[i].vertex[2] = float(m_vertices[i][2]);

		m_point_data[i].normal[0] = float(m_normals[i][0]);
		m_point_data[i].normal[1] = float(m_normals[i][1]);
		m_point_data[i].normal[2] = float(m_normals[i][2]);

		m_point_data[i].uv[0] = float(m_uvs[i][0]);
		m_point_data[i].uv[1] = float(m_uvs[i][1]);

	}

}

const std::vector<point_3d>& polymesh::aspoint() const
{
	return m_point_data;
}

void polymesh::gl_register(const program& in_program)
{
	if (events::getInstance().get_context() == false)
		return;
  if(m_registered == true)
    return;


  assert(in_program.get_program()!=0);
  glUseProgram(in_program.get_program());

  //check for equal size data buffers
  assert(is_valid());

  /////////////////////////BUFFERS//////////////////////////////////

  glGenVertexArrays(1,&m_vao);
  glBindVertexArray(m_vao);

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, aspoint().size() * sizeof(point_3d) , &aspoint()[0], GL_STATIC_DRAW);

  glGenBuffers(1, &m_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices().size(), &indices()[0], GL_STATIC_DRAW);


  m_pos_loc = glGetAttribLocation(in_program.get_program(), "vertices");
  m_normal_loc = glGetAttribLocation(in_program.get_program(), "normals");
  m_uv_loc = glGetAttribLocation(in_program.get_program(), "uvs");

  //std::cout<<"array location for m_pos: "<< m_pos_loc<<std::endl;
  //std::cout<<"array location for m_normal: "<< m_normal_loc<<std::endl;
  //std::cout<<"array location for m_uv: "<< m_uv_loc<<std::endl;

  //VERTICES
  glEnableVertexAttribArray(m_pos_loc);
  glVertexAttribPointer(m_pos_loc, 3, GL_FLOAT, GL_FALSE,
                        sizeof(point_3d), (void*)0);

  //NORMALS
  glEnableVertexAttribArray(m_normal_loc);
  glVertexAttribPointer(m_normal_loc, 3, GL_FLOAT, GL_TRUE,
                       sizeof(point_3d), (void*)offsetof(point_3d,point_3d::normal));

  //UVS
  glEnableVertexAttribArray(m_uv_loc);
  glVertexAttribPointer(m_uv_loc, 2, GL_FLOAT, GL_TRUE,
                       sizeof(point_3d), (void*)offsetof(point_3d,point_3d::uv));

  m_registered = true;

}

void polymesh::draw() const
{
  glBindVertexArray(m_vao);
  glEnableVertexAttribArray(m_pos_loc);
  glEnableVertexAttribArray(m_normal_loc);
  glEnableVertexAttribArray(m_uv_loc);
}

bool polymesh::is_registered() const
{
    return m_registered;
}

void polymesh::on_off()
{
    m_registered = 1-m_registered;
    glBindVertexArray(m_vao);
    glDisableVertexAttribArray(m_pos_loc);
    glDisableVertexAttribArray(m_normal_loc);
    glDisableVertexAttribArray(m_uv_loc);
}

void polymesh::free_mem()
{
    //FREES GL MEMORY
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBufferData(GL_ARRAY_BUFFER,0 , NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,0,NULL,GL_STATIC_DRAW);
    m_indices_size = 0;
}

const unsigned int& polymesh::size() const
{
  return m_indices_size;
}
polymesh::polymesh(polymesh&& target):
m_vertices(target.m_vertices), m_indices(target.m_indices),
m_normals(target.m_normals),m_uvs(target.m_uvs),
m_point_data(target.m_point_data),
m_registered(target.m_registered),m_vao(target.m_vao),
m_vbo(target.m_vbo), m_ibo(target.m_ibo),
m_pos_loc(target.m_pos_loc),m_normal_loc(target.m_normal_loc),
m_uv_loc(target.m_uv_loc),m_indices_size(target.m_indices_size)
{
  std::cout<<"polymesh move constructor called\n";

  register_to_events();


  const void * address = static_cast<const void*>(&target);
  std::stringstream ss;
  ss << address;
  std::string name = ss.str();

  events::getInstance().unregister_event(name);

  //FREE MEM + initialize to 0
  target.m_registered = 0;
  target.m_vao = 0;
  target.m_vbo = 0;
  target.m_ibo = 0;
  target.m_pos_loc = 0;
  target.m_normal_loc = 0;
  target.m_uv_loc = 0;
  target.m_indices_size = 0;
  target.m_vertices.clear();
  target.m_indices.clear();
  target.m_normals.clear();
  target.m_uvs.clear();
  target.m_point_data.clear();

}

polymesh& polymesh::operator=(polymesh &&target)
{
    //std::cout<<"polymesh move operator= called\n";

    if(this!=&target)
    {
      //FLUSH MEMORY
      release();

      //SWAP VALUES
      std::swap(m_registered,target.m_registered);
      std::swap(m_vao,target.m_vao);
      std::swap(m_vbo,target.m_vbo);
      std::swap(m_ibo,target.m_ibo);
      std::swap(m_pos_loc,target.m_pos_loc);
      std::swap(m_normal_loc,target.m_normal_loc);
      std::swap(m_uv_loc,target.m_uv_loc);
      std::swap(m_indices_size,target.m_indices_size);
      std::swap(m_vertices, target.m_vertices);
      std::swap(m_indices, target.m_indices);
      std::swap(m_normals, target.m_normals);
      std::swap(m_uvs, target.m_uvs);
      std::swap(m_point_data, target.m_point_data);

      //NEEDS TO UNREGISTER FROM EVENTS
    }
    return *this;
}

polymesh::~polymesh()
{
  //std::cout<<"polymesh destructor called\n";
  release();
}

void polymesh::release()
{
    //std::cout<<"release called\n";
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glDeleteBuffers(1,&m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glDeleteBuffers(1,&m_ibo);
    m_vbo = 0;
    m_ibo = 0;

}
void polymesh::print() const
{
    std::cout<<m_vao<<std::endl;
    std::cout<<m_vbo<<std::endl;
    std::cout<<m_ibo<<std::endl;
    std::cout<<m_pos_loc<<std::endl;
    std::cout<<m_normal_loc<<std::endl;
    std::cout<<m_uv_loc<<std::endl;
    std::cout<<m_registered<<std::endl;
	
	/*auto it = m_point_data.begin();
	while(it != m_point_data.end())
	{
		std::cout << "vertex is " << it->vertex[0] << " " << it->vertex[1] << " " << it->vertex[2] << std::endl;
		std::cout << "normal is " << it->normal[0] << " " << it->normal[1] << " " << it->normal[2] << std::endl;
		std::cout << "uv is " << it->uv[0] << " " << it->uv[1] << " " << std::endl;
		++it;
	}*/

}

void polymesh::set_position(const std::vector<glm::vec3>& in_points)
{
	m_vertices = in_points;	
}

void polymesh::set_indices(const std::vector<glm::vec3>& in_vertices)
{
	m_indices.clear();
	for (int i = 0; i < in_vertices.size(); i++)
	{
		m_indices.push_back(i);
	}

	m_indices_size = m_indices.size();
}
	
void polymesh::set_normals(const std::vector<glm::vec3>& in_normals)
{
	m_normals = in_normals;
}
void polymesh::set_uvs(const std::vector<glm::vec2>& in_uvs)
{
	m_uvs = in_uvs;
}

std::vector<glm::vec3> polymesh::unindexed(const std::vector<glm::vec3>& invertices, const std::vector<unsigned int>& inindices)
{

	assert(*(std::max_element(std::begin(inindices), std::end(inindices))) == invertices.size() - 1);
	//std::cout << "found compatible vector indices\n";

	std::vector<glm::vec3> outvertices;
	auto it = inindices.begin();

	while (it != inindices.end())
	{
		outvertices.push_back(invertices[*it]);
		++it;
	}

	return outvertices;
}


std::vector<glm::vec2> polymesh::unindexed(const std::vector<glm::vec2>& invertices, const std::vector<unsigned int>& inindices)
{
	
	assert(*(std::max_element(std::begin(inindices), std::end(inindices))) == invertices.size() - 1);
	//std::cout << "found compatible vector(2) indices\n";
	std::vector<glm::vec2> outvertices;
	auto it = inindices.begin();

	while (it != inindices.end())
	{
		outvertices.push_back(invertices[*it]);
		++it;
	}

	return outvertices;
}

void polymesh::swap_winding_order(std::vector<unsigned int> & indices,
	const std::vector<unsigned int>& counts)
{
	assert(is_valid(indices,counts));

	std::vector<unsigned int>::iterator from = indices.begin();
	std::vector<unsigned int>::iterator to = indices.begin();

	for (unsigned int a = 0; a<counts.size(); a++)
	{

		std::advance(to, counts[a]);
		std::reverse(from, to);
		std::advance(from, counts[a]);
	}

}

bool polymesh::is_valid(const std::vector<unsigned int>& vertIndices,
	const std::vector<unsigned int>& faceCounts)
{
	unsigned int total = 0;
	std::vector<unsigned int>::const_iterator it;

	for (it = faceCounts.begin(); it != faceCounts.end(); ++it)
	{
		total += *it;
	}

	//std::cout << "total faces: " << total << "\n";
	//std::cout << "total indices: " << vertIndices.size() << "\n";

	return(total == vertIndices.size());

}

}
