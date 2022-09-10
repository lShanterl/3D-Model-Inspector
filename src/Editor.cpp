#include "Editor.hpp"



void editor::SetEditor(void* light, LightType type, int index)
{
	m_LightPointer = light;
	m_LightType = type;
	index =m_index;
}

void editor::RenderLightButtons(std::vector<PointLight>& pointLights, std::vector<DirLight>& dirLights)
{
	
	if (ImGui::CollapsingHeader("Lights"))
	{
		if (ImGui::Button("add pointLight"))
		{
			pointLights.emplace_back(PointLight{});
		}
		ImGui::SameLine();
		if (ImGui::Button("add dirLight"))
		{
			dirLights.emplace_back(DirLight{});
		}
		
		for (int i = 0; i < pointLights.size(); i++)
		{

			std::string buttonName = "pointLight: " + std::to_string(i);
			if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowWidth() - 15, 35)))
			{
				editor::SetEditor(&pointLights[i], pointLights[i].m_type, i);
			}

		}
		for (int i = 0; i < dirLights.size(); i++)
		{
			std::string buttonName = "dirLight: " + std::to_string(i);
			
			if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowWidth() - 15, 35)))
			{
				editor::SetEditor(&dirLights[i], dirLights[i].m_type, i);
		
		
			}
		}

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
			ImGui::SliderFloat3("direction", (float*)&light->m_Direction[0], -5.0f, 5.0f);
			ImGui::ColorEdit3("light color", (float*)&light->m_LightCol[0]);
			if (ImGui::Button("delete"))
			{
				dirLights.erase(dirLights.begin() + m_index);
				m_LightPointer = nullptr;
				m_LightType = e_none;
			}

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



