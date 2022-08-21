#pragma once

#ifndef MODEL_H
#define MODEL_H
#include "Commons.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.hpp"
#include "Vertex.h"
#include "Mesh.hpp"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

class Model
{
public:
	Model(const std::string&& location);
	
	void CreateModel();

	VertexArray* m_vao;
	IndexBuffer* m_ib;
	Shader* m_shader;
	Texture* m_texture;

	void LoadTexture(const std::string&& path);

private:
	std::string m_location;
	glm::vec3 vertPos;	
};
#endif