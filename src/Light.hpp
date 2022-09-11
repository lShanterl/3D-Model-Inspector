
#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Commons.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

enum LightType
{
	e_dirLight,
	e_pointLight,
	e_spotLight,
	e_none
};

class DirLight
{
public:

	glm::vec3 m_Direction = { -0.2f, -1.0f, -0.3f };
	LightType m_type = e_dirLight;
	glm::vec3 m_LightCol = glm::vec3(1.0f, 1.0f, 1.0f);
	bool isWorking = true;


private:

};

class PointLight
{
public:

	glm::vec3 m_lightPos = glm::vec3(-.250f, 0.310f, 1.0f);

	glm::vec3 m_LightCol = glm::vec3(1.0f, 1.0f, 1.0f);

	float intensity = 1;

	bool isWorking = true;
	float ambientStrength = 0.1;

	float linear = 0.027f;
	float quadratic = 0.028f;

	LightType m_type = e_pointLight;

	glm::vec3 Getlightpos();
};

class SpotLight
{
public:

	glm::vec3 m_lightPos = glm::vec3(-.250f, 0.310f, 1.0f);
	glm::vec3 m_Direction = { -0.2f, -1.0f, -0.3f };

	float cutOff = 23.5f;
	float outerCutOff = 27.5f;

	float linear = 0.027f;
	float quadratic = 0.028f;
	//bool cameraMimic = false;

	glm::vec3 m_LightCol = glm::vec3(1.0f, 1.0f, 1.0f);

	bool isWorking = true;

	LightType m_type = e_spotLight;
};

#endif