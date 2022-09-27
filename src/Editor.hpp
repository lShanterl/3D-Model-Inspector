#pragma once

#ifndef EDITOR_HPP
#define EDITOR_HPP


#include "Camera.hpp"
#include "SceneObjects.hpp"


#include "Commons.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"



#include <string>
#include <vector>


namespace gui
{
	inline void* m_EditorPointer;
	inline ObjectType m_Type;
	inline int m_index;

	void SetEditor(void* object, ObjectType type, int index);
	void RenderLightButtons(Camera* camera);
}

#endif // !EDITOR_HPP
