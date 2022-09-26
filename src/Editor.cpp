#include "Editor.hpp"


void editor::SetEditor(void* object, ObjectType type, int index)
{
	m_EditorPointer = object;
	m_Type = type;
	m_index = index;
}

void editor::RenderLightButtons(Camera* camera)
{
	ImGuiWindowFlags window_flags = 0;
	window_flags =
		ImGuiWindowFlags_NoTitleBar;
	//ImGuiWindowFlags_NoMove;

	//ImGui::BeginMainMenuBar();
	ImGui::Begin("Model Inspector", 0, window_flags);
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Scene"))
		{

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::Separator();

	if (ImGui::TreeNode("Camera"))
	{
		ImGui::DragFloat("Movement speed", &camera->m_movementSpeed, 0.1f, 0.1f, 25.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::TreePop();
	}
	if (ImGui::CollapsingHeader("Lights"))
	{
		if (ImGui::Button("add pointLight"))
		{
			scene::m_PointLights.emplace_back(PointLight{});
		}
		ImGui::SameLine();
		if (ImGui::Button("add dirLight"))
		{
			scene::m_DirectionalLights.emplace_back(DirLight{});
		}
		ImGui::SameLine();
		if (ImGui::Button("add spotLight"))
		{
			scene::m_SpotLights.emplace_back(SpotLight{});
		}
		
		for (int i = 0; i < scene::m_PointLights.size(); i++)
		{

			std::string buttonName = "pointLight: " + std::to_string(i);
			if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowWidth() - 15, 35)))
			{
				editor::SetEditor(&scene::m_PointLights[i], scene::m_PointLights[i].m_type, i);
			}

		}
		for (int i = 0; i < scene::m_DirectionalLights.size(); i++)
		{
			std::string buttonName = "dirLight: " + std::to_string(i);	
			if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowWidth() - 15, 35)))
			{
				editor::SetEditor(&scene::m_DirectionalLights[i], scene::m_DirectionalLights[i].m_type, i);
			}
		}
		for (int i = 0; i < scene::m_SpotLights.size(); i++)
		{
			std::string buttonName = "spotLight: " + std::to_string(i);
			if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowWidth() - 15, 35)))
			{
				editor::SetEditor(&scene::m_SpotLights[i], scene::m_SpotLights[i].m_type, i);
			}
		}

	}
	
	if (ImGui::CollapsingHeader("Models"))
	{

		if (ImGui::Button("add model"))
		{
			m_EditorPointer = nullptr; // i need to do that so it doesnt crash when i add a model
			sl::BeginBenchmark("modelCreator");
			scene::m_Models.emplace_back(scene::GlobalShader, "res/models/backpack.obj");
			scene::m_Models.back().m_material->SetTextures("res/textures/backpack_albedo.jpg", "res/textures/backpack_specular.jpg");
			std::cout << "mesh loaded in: " << sl::EndBenchmark("modelCreator") << "seconds\n";
		}
		for (int i = 0; i < scene::m_Models.size(); i++)
		{

			std::string buttonName = "model: " + std::to_string(i);
			if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowWidth() - 15, 35)))
			{
				editor::SetEditor(&scene::m_Models[i], scene::m_Models[i].m_type, i);

			}


		}
	}
	ImGui::End();
	ImGui::Begin("Editor");
	if (m_EditorPointer)
	{

		if (m_Type == e_pointLight)
		{

			PointLight* light = reinterpret_cast<PointLight*>(m_EditorPointer);

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
				scene::m_PointLights.erase(scene::m_PointLights.begin() + m_index);
				m_EditorPointer = nullptr;
				m_Type = e_none;
			}
		}
		else if (m_Type == e_dirLight)
		{
			DirLight* light = reinterpret_cast<DirLight*>(m_EditorPointer);

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
				scene::m_DirectionalLights.erase(scene::m_DirectionalLights.begin() + m_index);
				m_EditorPointer = nullptr;
				m_Type = e_none;
			}

		}
		else if (m_Type == e_spotLight)
		{
			SpotLight* light = reinterpret_cast<SpotLight*>(m_EditorPointer);

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
				scene::m_SpotLights.erase(scene::m_SpotLights.begin() + m_index);
				m_EditorPointer = nullptr;
				m_Type = e_none;
			}
		}
		else if (m_Type == e_model)
		{
			Model* model = reinterpret_cast<Model*>(m_EditorPointer);
			std::string name = "mesh: " + std::to_string(m_index);
			ImGui::Text(name.c_str());
			ImGui::Spacing();

			ImGui::SliderFloat3("Position", &model->position[0], -10.01f, 10.01f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::SliderFloat3("Scale", &model->scale[0], -10.01f, 10.01f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::SliderFloat3("Rotation", &model->rotation[0], -180.0f, 180.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::Spacing();

			ImGui::Checkbox("Use Albedo Texture", &model->m_material->m_useAlbedo);
			ImGui::Checkbox("Use Specular Texture", &model->m_material->m_useSpecular);	

			ImGui::InputText("model path", &model->m_FilePath[0], sizeof(char) * 65);
			if (model->m_material->m_useAlbedo)
			{
				if (ImGui::InputText("albedo texture path", model->m_material->albedoTexPath, sizeof(char) * 65));
			}
			if (model->m_material->m_useSpecular)
			{
				if (ImGui::InputText("specular texture path", model->m_material->specularTexPath, sizeof(char) * 65));

			}
			if (ImGui::Button("Load Model and Texture"))
			{
				if (model->m_material->albedoTexture != nullptr)
					delete(model->m_material->albedoTexture);
				if (model->m_material->specularTexture != nullptr)
					delete(model->m_material->specularTexture);
				
				if (model->m_material->m_useAlbedo)
				{
					model->m_material->albedoTexture = new Texture(model->m_material->albedoTexPath);
				}
				else
				{
					unsigned char white[4] = { 255,255,255,255 };
					model->m_material->albedoTexture = new Texture(white, 1, 1);
				}
				if (model->m_material->m_useSpecular)
				{
					model->m_material->specularTexture = new Texture(model->m_material->specularTexPath);
				}
				else
				{
					unsigned char darkGray[4] = { 36,36,36,255 };
					model->m_material->specularTexture = new Texture(darkGray, 1, 1);
				}

			}
		}
	}
		ImGui::End();
}



