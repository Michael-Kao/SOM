#include "Window.h"

void KeyboardCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Window::Window(unsigned int width, unsigned int height, const char* title)
	: m_width(width), m_height(height), camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
    lastX = m_width / 2.0f;
    lastY = m_height / 2.0f;
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, title, NULL, NULL);
    if (m_window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        exit(-1);
    }
    glfwSetWindowUserPointer(m_window, this);
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, FrameBufferSizeCallBack);
    glfwSetKeyCallback(m_window, KeyboardCallBack);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallBack);
    glfwSetCursorPosCallback(m_window, mouse_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        exit(-1);
    }
}

void KeyboardCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* wndClass = (Window*)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS) {
        wndClass->m_keyPressed[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        wndClass->m_keyPressed[key] = false;
    }
}

void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    Window* wndClass = (Window*)glfwGetWindowUserPointer(window);
    wndClass->m_width = width;
    wndClass->m_height = height;
    glViewport(0, 0, wndClass->m_width, wndClass->m_height);
}

void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{

}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    Window* wndClass = (Window*)glfwGetWindowUserPointer(window);
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (wndClass->firstMouse)
    {
        wndClass->lastX = xpos;
        wndClass->lastY = ypos;
        wndClass->firstMouse = false;
    }

    float xoffset = xpos - wndClass->lastX;
    float yoffset = wndClass->lastY - ypos; // reversed since y-coordinates go from bottom to top

    wndClass->lastX = xpos;
    wndClass->lastY = ypos;

    wndClass->camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* wndClass = (Window*)glfwGetWindowUserPointer(window);
    wndClass->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}