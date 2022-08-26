
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
	float shininess;
	glm::vec3 diffuse;

};

#endif // !MATERIAL_HPP
