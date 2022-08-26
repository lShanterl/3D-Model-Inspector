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
#include "Vertex.h"
#include "Material.hpp"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "SimpleLogging/SimpleLogging.hpp"

class Model
{
public:
	Model(const std::string&& location, Material* material);

	VertexArray* m_vao;
	IndexBuffer* m_ib;
	Material* m_material;

	void LoadTexture(const std::string&& path);

private:
	std::string m_FilePath;
	std::string m_TexturePath;
	glm::vec3 vertPos;	
};
#endif