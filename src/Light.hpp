
#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Commons.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

class Light
{
	//vec3 lightDir = normalize(lightDir - FragPos);
public:
	Light();
	~Light();

	glm::vec3 m_lightPos = glm::vec3(-.250f, 0.310f, 1.0f);

	glm::vec3 m_LightCol = glm::vec3(1.0f,1.0f,1.0f);

	float ambientStrength = 0.1;
	glm::vec3 Getlightpos();

    
private:
	
};


class DirLight
{
	//vec3 lightDir = normalize(-lightDir);
	//uniform vec3 position; no longer necessary when using directional lights.
public:

	glm::vec3 m_Direction = { -0.2f, -1.0f, -0.3f };
	float ambient;


private:

};

class PointLight
{
public:

	glm::vec3 m_lightPos = glm::vec3(-.250f, 0.310f, 1.0f);

	glm::vec3 m_LightCol = glm::vec3(1.0f, 1.0f, 1.0f);

	float intensity = 1;

	bool isWorking = false;

	float ambientStrength = 0.1;

	float linear = 0.027f;
	float quadratic = 0.028f;

	glm::vec3 Getlightpos();
};

class SpotLight
{
public:



};

#endif