all: wolfenstein.o events.o polymesh.o opengl.o matrices.o glfw.o
	g++ -std=c++11 -Wall -I/tools/SITE/rnd/include/glad/glad-2016.9.30 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3 -I/tools/SITE/rnd/include/glfw/glfw-3.2.1 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3/ wolfenstein.o events.o polymesh.o opengl.o matrices.o glfw.o -o wolfenstein -L/tools/SITE/rnd/lib/Linux64/glfw/glfw-3.2.1 -L/tools/SITE/rnd/lib/Linux64/glad/glad-2016.9.30 -ldl -lGL -lGLU -lglad -l:libglfw.so
wolfenstein.o: wolfenstein.cpp events.hpp
	g++ -std=c++11 -Wall -I/tools/SITE/rnd/include/glad/glad-2016.9.30 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3 -I/tools/SITE/rnd/include/glfw/glfw-3.2.1 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3/ -c wolfenstein.cpp
events.o: events.cpp events.hpp
	g++ -std=c++11 -Wall -I/tools/SITE/rnd/include/glad/glad-2016.9.30 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3 -I/tools/SITE/rnd/include/glfw/glfw-3.2.1 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3/ -c events.cpp
polymesh.o: polymesh.cpp polymesh.hpp
	g++ -std=c++11 -Wall -I/tools/SITE/rnd/include/glad/glad-2016.9.30 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3 -I/tools/SITE/rnd/include/glfw/glfw-3.2.1 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3/ -c polymesh.cpp
opengl.o: opengl.cpp opengl.hpp polymesh.hpp
	g++ -std=c++11 -Wall -I/tools/SITE/rnd/include/glad/glad-2016.9.30 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3 -I/tools/SITE/rnd/include/glfw/glfw-3.2.1 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3/ -c opengl.cpp
matrices.o: matrices.cpp matrices.hpp
	g++ -std=c++11 -Wall -I/tools/SITE/rnd/include/glad/glad-2016.9.30 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3 -I/tools/SITE/rnd/include/glfw/glfw-3.2.1 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3/ -c matrices.cpp
glfw.o: glfw.cpp glfw.hpp
	g++ -std=c++11 -Wall -I/tools/SITE/rnd/include/glad/glad-2016.9.30 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3 -I/tools/SITE/rnd/include/glfw/glfw-3.2.1 -I/tools/SITE/rnd/include/glm/glm-0.9.8.3/ -c glfw.cpp	

clean:
	rm wolfenstein.o events.o polymesh.o opengl.o matrices.o glfw.o wolfenstein 
