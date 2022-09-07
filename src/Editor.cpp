#include "Editor.hpp"



void editor::SetEditor(void* light, LightType type, int index)
{
	m_LightPointer = light;
	m_LightType = type;
	index =m_index;
}

void editor::Renderer()
{
	if (!m_LightPointer)
		return;

	if (m_LightType == e_pointLight)
	{
		PointLight* light = reinterpret_cast<PointLight*>(m_LightPointer);
		RenderPoint(light);
	}
	else if(m_LightType == e_dirLight)
	{
		DirLight* light = reinterpret_cast<DirLight*>(m_LightPointer);
		RenderDir(light);
	}
	else if(m_LightType == e_spotLight)
	{
		SpotLight* light = reinterpret_cast<SpotLight*>(m_LightPointer);
		RenderSpot(light);
	}
	else
	{

	}
}

void editor::RenderLightButtons(std::vector<PointLight>& pointLights)
{
	
	if (ImGui::CollapsingHeader("Lights", ImGuiStyleVar_PopupRounding))
	{
		if (ImGui::Button("add pointLight"))
		{
			pointLights.emplace_back(PointLight{});
		}
		//if (ImGui::Button("add dirLight"))
		//{
		//	dirLightAmount++;
		//	dirLights[dirLightAmount] = new DirLight;
		//}
		
		for (int i = 0; i < pointLights.size(); i++)
		{

			std::string buttonName = "pointLight: " + std::to_string(i);
			if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowWidth(), 35)))
			{
				editor::SetEditor(&pointLights[i], pointLights[i].m_type, i);
			}
		}
		//for (int i = 0; i <= dirLightAmount; i++)
		//{
		//	std::string buttonName = "dirLight: " + std::to_string(i);
		//	if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowWidth(), 35)))
		//	{
		//		editor::SetEditor(dirLights[i], dirLights[i]->m_type);
		//
		//
		//	}
		//}

	}


	ImGui::Begin("Editor");
	if (m_LightPointer)
	{
		if (m_LightType == e_pointLight)
		{

			PointLight* light = reinterpret_cast<PointLight*>(m_LightPointer);
			ImGui::SliderFloat3("position", (float*)&light->m_lightPos[0], -5.0f, 5.0f);
			ImGui::ColorEdit3("light color", (float*)&light->m_LightCol[0]);
			if (ImGui::Button("delete"))
			{
				pointLights.erase(pointLights.begin() + m_index);
				m_LightPointer = nullptr;
				m_LightType = e_none;
			}
		}
		else if (m_LightType == e_dirLight)
		{
			DirLight* light = reinterpret_cast<DirLight*>(m_LightPointer);

		}
		else if (m_LightType == e_spotLight)
		{
			SpotLight* light = reinterpret_cast<SpotLight*>(m_LightPointer);

		}
		else
		{

		}
	}
		
	
	

	ImGui::End();

}

void editor::RenderDir(DirLight* light)
{
	ImGui::SliderFloat3("direction", (float*)&light->m_Direction[0], -5.0f, 5.0f);
	ImGui::ColorEdit3("light color", (float*)&light->m_LightCol[0]);
}

void editor::RenderPoint(PointLight* light)
{
	ImGui::SliderFloat3("position", (float*)&light->m_lightPos[0], -5.0f, 5.0f);
	ImGui::ColorEdit3("light color", (float*)&light->m_LightCol[0]);
	if(ImGui::Button("delete"))
	{		
		
		m_LightPointer = nullptr;
		m_LightType = e_none;
		
		
	}
	
}

void editor::RenderSpot(SpotLight* light)
{

}


