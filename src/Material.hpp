
#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Texture.hpp"
#include "Shader.h"

struct Material
{
	Material(Shader* shader)
		:m_shader(shader)
	{
		albedoTexture = nullptr;
		specularTexture = nullptr;
	}
	~Material()
	{

	}

	Texture* albedoTexture;
	Texture* specularTexture;
	Shader* m_shader;

	void SetTextures(const char* albedo, const char* specular)
	{
		if(albedoTexture != nullptr)
			delete(albedoTexture);
		if(specularTexture != nullptr)
			delete(specularTexture);
		specularTexture = new Texture(specular);
		albedoTexture =   new Texture(albedo);  
	}

	bool m_useAlbedo = false;
	bool m_useSpecular = false;

	char modelPath[65] = "";
	char albedoTexPath[65] = "";
	char specularTexPath[65] = "";

	float shininess = 24.0f;
};

#endif // !MATERIAL_HPP
