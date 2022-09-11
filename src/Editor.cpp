#include "Editor.hpp"



void editor::SetEditor(void* light, LightType type, int index)
{
	m_LightPointer = light;
	m_LightType = type;
	m_index = index;
}

void editor::RenderLightButtons(std::vector<PointLight>& pointLights, std::vector<DirLight>& dirLights, std::vector<SpotLight>& spotLights, Camera* camera)
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
		ImGui::SameLine();
		if (ImGui::Button("add spotLight"))
		{
			spotLights.emplace_back(SpotLight{});
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
		for (int i = 0; i < spotLights.size(); i++)
		{
			std::string buttonName = "spotLight: " + std::to_string(i);
			if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowWidth() - 15, 35)))
			{
				editor::SetEditor(&spotLights[i], spotLights[i].m_type, i);
			}
		}

	}


	ImGui::Begin("Editor");
	if (m_LightPointer)
	{
		if (m_LightType == e_pointLight)
		{

			PointLight* light = reinterpret_cast<PointLight*>(m_LightPointer);

			std::string name = "pointLight: " + std::to_string(m_index);

			ImGui::Text(name.c_str());
			ImGui::SliderFloat3("position", (float*)&light->m_lightPos[0], -5.0f, 5.0f);
			ImGui::ColorEdit3("light color", (float*)&light->m_LightCol[0]);
			if (ImGui::Button("Set camera's transform"))
			{
				light->m_lightPos = camera->m_Position;
			}
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

			std::string name = "dirLight: " + std::to_string(m_index);

			ImGui::Text(name.c_str());
			ImGui::SliderFloat3("direction", (float*)&light->m_Direction[0], -5.0f, 5.0f);
			ImGui::ColorEdit3("light color", (float*)&light->m_LightCol[0]);
			if (ImGui::Button("Set camera's transform"))
			{
				light->m_Direction = camera->cameraFront;
			}
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
			
			std::string name = "spotLight: " + std::to_string(m_index);

			ImGui::Text(name.c_str());
			ImGui::SliderFloat3("direction", (float*)&light->m_Direction[0], -5.0f, 5.0f);
			ImGui::SliderFloat3("position", (float*)&light->m_lightPos[0], -5.0f, 5.0f);
			ImGui::ColorEdit3("light color", (float*)&light->m_LightCol[0]);
			ImGui::SliderFloat("cutOff", &light->cutOff, 0, 90);
			ImGui::SliderFloat("outerCutOff", &light->outerCutOff, 0, 180);
			
			//ImGui::Checkbox("Control light by camera", &light->cameraMimic);
			if (ImGui::Button("Set camera's transform"))
			{
				light->m_Direction = camera->cameraFront;
				light->m_lightPos = camera->m_Position;
			}
			
			if (ImGui::Button("delete"))
			{
				spotLights.erase(spotLights.begin() + m_index);
				m_LightPointer = nullptr;
				m_LightType = e_none;
			}
		}
		else
		{

		}
	}
		
	
	

	ImGui::End();

}



