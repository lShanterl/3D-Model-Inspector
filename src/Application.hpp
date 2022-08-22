#ifndef APPLICATION_HPP
#define APPLICATION_HPP

// GLEW / GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//C++
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <memory>

//GLM
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

//Own Header Files
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Model.h"
#include "Light.hpp"


namespace Application
{

	void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void Init();
	void Update();
	void Render();
	void Exit();
	
}


#endif // !APPLICATION_HPP
