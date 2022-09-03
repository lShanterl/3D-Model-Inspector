#include "Light.hpp"

Light::Light()
{
	
}

Light::~Light()
{
}

glm::vec3 Light::Getlightpos()
{
	return m_lightPos;
}

glm::vec3 PointLight::Getlightpos()
{
	return m_lightPos;
}
