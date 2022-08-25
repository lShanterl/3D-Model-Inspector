
#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Commons.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

class Light
{
public:
	Light();
	~Light();

	glm::vec3 m_lightDir = glm::vec3(2.5f, 1.0f, 2.0f);

	glm::vec3 m_LightCol = glm::vec3(1.0f,1.0f,1.0f);

	glm::vec3 intensity;
	float ambientStrength = 0.1;
	glm::vec3 GetlightDir();

    
private:
	
};

#endif