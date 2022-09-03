#pragma once

#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "Commons.h"
#include <GLFW/glfw3.h>


#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
class Camera {
public:
	float m_movementSpeed = 5.5f;
	const float m_mouseSensitivity = 0.20f;
	float m_Yaw = -90.0f;
	float m_Pitch = 0.0f;
	float m_Fov;
	bool isFocused = false;

	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_Position;

	Camera(glm::vec3 pos, float fov, float lastX, float lastY);
		
	glm::mat4 GetProj(int width, int height);

	glm::mat4 GetView();

	void ProcessInput(GLFWwindow* window);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypo);

	void CameraFocusing(GLFWwindow* window);

private:
	float m_lastX, m_lastY;
	bool m_firstMouse = true;

};
#endif