
#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Texture.hpp"
#include "Shader.h"


struct Material
{
	Material(Texture* albedo, Texture* specular, Shader* shader)
		:albedoTexture(albedo), specularTexture(specular), m_shader(shader)
	{
	}
	~Material()
	{

	}

	Texture* albedoTexture;
	Texture* specularTexture;
	Shader* m_shader;
	float shininess = 24.0f;
	glm::vec3 ambient = glm::vec3(0.04);
	glm::vec3 specular = { 0.5f, 0.5f, 0.5f };


};

#endif // !MATERIAL_HPP
