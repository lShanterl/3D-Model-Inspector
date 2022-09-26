#pragma once


#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include "SceneObjects.hpp"
#include "Camera.hpp"

class Renderer
{
public:
	Renderer(Camera* camera);
	void Clear();
	void SetClearColor(float r, float g, float b, float a);
	void SetUniformsAndRender();
private:
	void Draw(Model& mesh);
	glm::mat4 view;
	Camera* camera;
	glm::mat4 proj;
	
};

#endif