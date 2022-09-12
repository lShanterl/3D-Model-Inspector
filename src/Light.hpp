
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

struct DirLight
{
	glm::vec3 m_LightCol = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 m_Direction = { -0.2f, -1.0f, -0.3f };
	bool isWorking = true;

	LightType m_type = e_dirLight;
};

struct PointLight
{
	glm::vec3 m_lightPos = glm::vec3(-.250f, 0.310f, 1.0f);

	glm::vec3 m_LightCol = glm::vec3(1.0f, 1.0f, 1.0f);

	float intensity = 1;

	bool isWorking = true;

	LightType m_type = e_pointLight;
};

struct SpotLight
{
	glm::vec3 m_lightPos = glm::vec3(-.250f, 0.310f, 1.0f);

	glm::vec3 m_Direction = { -0.2f, -1.0f, -0.3f };

	glm::vec3 m_LightCol = glm::vec3(1.0f, 1.0f, 1.0f);

	float cutOff = 23.5f;
	float outerCutOff = 27.5f;

	bool isWorking = true;
	LightType m_type = e_spotLight;
};

#endif