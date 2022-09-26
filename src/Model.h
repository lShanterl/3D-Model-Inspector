#pragma once

#ifndef MODEL_H
#define MODEL_H
#include "Commons.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Vertex.h"
#include "Material.hpp"
#include "Light.hpp"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "SimpleLogging/SimpleLogging.hpp"

class Model
{
public:
	Model(Shader* shader, const std::string location);

	VertexArray* m_vao;
	IndexBuffer* m_ib;
	Material* m_material;
	glm::mat4 modelView = glm::mat4(1.0f);

	void CreateMesh(const std::string location);

	ObjectType m_type = e_model;

	std::string m_FilePath;

	glm::vec3 position  = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);
	glm::vec3 lastPos = glm::vec3(0);

private:
	std::string m_TexturePath;
	glm::vec3 vertPos;	
};
#endif