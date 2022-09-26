
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

	char modelPath[100] = "";
	char albedoTexPath[100] = "";
	char specularTexPath[100] = "";

	float shininess = 24.0f;
};

#endif // !MATERIAL_HPP
