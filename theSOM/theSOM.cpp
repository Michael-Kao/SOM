#include "theSOM.h"
#include "Window.h"
#include "Shader.h"
#include "som.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* TODO:
	https://math.stackexchange.com/questions/1488345/what-is-the-shortest-distance-between-two-2d-points-on-the-surface-of-a-cylinder
*/

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void procssInput(Window* window);

int main()
{
	Window m_window(WIDTH, HEIGHT, "SOM");
	Shader shader("shaders/vert.glsl", "shaders/frag.glsl"), cshader("shaders/vert.glsl", "shaders/cfrag.glsl");
	glEnable(GL_DEPTH_TEST);
	SOM m_SOM(11, 11, WIDTH, HEIGHT, 100000);
	m_SOM.ReadTestData("objfiles\\b.obj");
	while (m_window.ProcessEvent())
	{
		//per-frame time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		procssInput(&m_window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = m_window.camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(m_window.camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
		glm::mat4 MODELVIEWPROJECTION = projection * view * model;
		shader.setMat4("MODELVIEWPROJECTION", MODELVIEWPROJECTION);
		m_SOM.TestDraw();
		cshader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		MODELVIEWPROJECTION = projection * view * model;
		cshader.setMat4("MODELVIEWPROJECTION", MODELVIEWPROJECTION);
		m_SOM.Render();
		if (!m_SOM.isFinished())
		{
			m_SOM.Train();
		}
		if (m_window.KeyDown(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(m_window.GetWindow(), true);
		}
		m_window.SwapBuffers();
	}
	glfwTerminate();
	return 0;
}

void procssInput(Window* window)
{
	if(window->KeyDown(GLFW_PRESS))
		glfwSetWindowShouldClose(window->GetWindow(), true);
	if (window->KeyDown(GLFW_KEY_W))
		window->camera.ProcessKeyboard(FORWARD, deltaTime);
	if (window->KeyDown(GLFW_KEY_S))
		window->camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (window->KeyDown(GLFW_KEY_A))
		window->camera.ProcessKeyboard(LEFT, deltaTime);
	if (window->KeyDown(GLFW_KEY_D))
		window->camera.ProcessKeyboard(RIGHT, deltaTime);
}