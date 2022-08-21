
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

	glm::vec3 m_lightPos = glm::vec3(3.2f, 1.0f, 2.0f);
	glm::vec3 m_LightCol;
	glm::vec3 intensity;
	float ambientStrength = 0.1;
	glm::vec3 GetLightPos();

    
private:
	
};

#endif