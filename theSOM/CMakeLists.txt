
add_executable ( theSOM 
	"theSOM.cpp" 
	"Window.cpp" 
	"som.cpp" 
	)

target_link_libraries ( theSOM
		OpenGL::GL
		glfw
		glad
		glm::glm
 )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET theSOM PROPERTY CXX_STANDARD 20)
endif()
