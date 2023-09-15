#pragma once

#include "theSOM.h"
#include "camera.h"

class Window
{
public:
	Window(unsigned int width, unsigned int height, const char* title);
	Camera camera;
	bool ProcessEvent()
	{
		glfwPollEvents();
		return glfwWindowShouldClose(m_window) == 0;
	}
	void SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}
	bool KeyDown(int key)
	{
		return m_keyPressed[key];
	}
	GLFWwindow* GetWindow()
	{
		return m_window;
	}
private:
	float lastX;
	float lastY;
	bool firstMouse = false;
	GLFWwindow* m_window;
	unsigned int m_width;
	unsigned int m_height;
	std::array<bool, GLFW_KEY_LAST + 1> m_keyPressed{ 0 };
	friend void KeyboardCallBack(GLFWwindow* window, int key, int scanecode, int action, int mods);
	friend void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
	friend void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
	friend void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};