#pragma once

#ifndef SCENEOBJECTS_HPP
#define SCENEOBJECTS_HPP
#include <vector>

#include "Light.hpp"
#include "Material.hpp"
#include "Model.h"


namespace scene
{
	inline Shader* GlobalShader;
	inline extern glm::vec3 ambient = glm::vec3(0.02);
	inline std::vector<PointLight> m_PointLights;
	inline std::vector<DirLight> m_DirectionalLights;
	inline std::vector<SpotLight> m_SpotLights;
	inline std::vector<Shader> m_Shaders;
	inline std::vector<Model> m_Models;
};

#endif // !SCENEOBJECTS_HPP

