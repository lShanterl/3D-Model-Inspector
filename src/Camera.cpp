#include "Camera.hpp"

Camera* currentCamera;

Camera::Camera(glm::vec3 pos, float fov, float lastX, float lastY)
	: m_Position(pos), m_Fov(fov), m_lastX(lastX/2), m_lastY(lastY/2)
{
    currentCamera = this;
}

glm::mat4 Camera::GetProj(int width, int height)
{
	return glm::perspective(glm::radians(m_Fov), static_cast<float>(width) / height, 0.001f, 2000.0f);
}

glm::mat4 Camera::GetView()
{
	return glm::lookAt(m_Position, m_Position + cameraFront, cameraUp);
}
void Camera::ProcessInput(GLFWwindow* window)
{
        float deltaTime = 0.0f;
        static float lastFrame = 0.0f;
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float m_Speed = m_movementSpeed * deltaTime;
    if (isFocused)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_Position += m_Speed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_Position -= m_Speed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_Position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * m_Speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_Position += glm::normalize(glm::cross(cameraFront, cameraUp)) * m_Speed;
    }
    
}


void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (currentCamera->isFocused == false)
    {
        currentCamera->m_lastX = xpos;
        currentCamera->m_lastY = ypos;
        return;
    }

    float xoffset = xpos - currentCamera->m_lastX;
    float yoffset = currentCamera->m_lastY - ypos; // reversed since y-coordinates range from bottom to top
    currentCamera->m_lastX = xpos;
    currentCamera->m_lastY = ypos;
    xoffset *= currentCamera->m_mouseSensitivity;
    yoffset *= currentCamera->m_mouseSensitivity;
    currentCamera->m_Yaw += xoffset;
    currentCamera->m_Pitch += yoffset;


    if (currentCamera->m_Pitch > 89.0f)
        currentCamera->m_Pitch = 89.0f;
    if (currentCamera->m_Pitch < -89.0f)
        currentCamera->m_Pitch = -89.0f;
    glm::vec3 direction;

    direction.x = cos(glm::radians(currentCamera->m_Yaw)) * cos(glm::radians(currentCamera->m_Pitch));
    direction.y = sin(glm::radians(currentCamera->m_Pitch));
    direction.z = sin(glm::radians(currentCamera->m_Yaw)) * cos(glm::radians(currentCamera->m_Pitch));
    currentCamera->cameraFront = glm::normalize(direction);

    
}


void Camera::CameraFocusing(GLFWwindow* window)
{
    //if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        isFocused = true;

    }else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        isFocused = false;

    }
}
