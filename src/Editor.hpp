#pragma once

#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "Light.hpp"
#include "Camera.hpp"

#include "Commons.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"



#include <string>
#include <vector>

namespace editor
{
	inline void* m_LightPointer;
	inline LightType m_LightType;
	inline int m_index;

	void SetEditor(void* light, LightType type, int index);
	void RenderLightButtons(std::vector<PointLight>& pointLights, std::vector<DirLight>& dirLights, std::vector<SpotLight>& spotLights, Camera* camera);
}

#endif // !EDITOR_HPP
