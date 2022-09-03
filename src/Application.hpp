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
#include <array>

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
#include "Material.hpp"


namespace Application
{

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	void Init();
	void Update();
	void Exit();
	bool IsRunning();
	
}


#endif // !APPLICATION_HPP
