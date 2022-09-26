#include "Renderer.h"
#include <iostream>

Renderer::Renderer(Camera* camera)
	:camera(camera)
{
    view = camera->GetView();
}

void Renderer::Draw(Model& model)
{
	model.m_vao->Bind();
    //ib.Bind();
    model.m_material->m_shader->Bind();
	
	if (model.m_material->albedoTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		model.m_material->albedoTexture->Bind(0);
		model.m_material->m_shader->SetInt("material.diffuse", 0);
	}
	if (model.m_material->specularTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		model.m_material->specularTexture->Bind(1);
		model.m_material->m_shader->SetInt("material.specular", 1);
	}

	glDrawArrays(GL_TRIANGLES, 0, model.m_vao->GetCount());
}
void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Renderer::SetUniformsAndRender()
{
    const float linear = 0.027f;
    const float quadratic = 0.028f;
    proj = camera->GetProj(camera->width, camera->height);

    scene::GlobalShader->SetVec3f("viewPos", camera->m_Position.x, camera->m_Position.y, camera->m_Position.z);

    scene::GlobalShader->SetInt("scene.mPointLights", scene::m_PointLights.size());
    scene::GlobalShader->SetInt("scene.mDirLights", scene::m_DirectionalLights.size());
	
    for (uint32_t i = 0; i < scene::m_PointLights.size(); i++)
    {

        if (scene::m_PointLights[i].isWorking)
        {
            scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].position"), scene::m_PointLights[i].m_lightPos.x, scene::m_PointLights[i].m_lightPos.y, scene::m_PointLights[i].m_lightPos.z);
            scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].lightColor"), scene::m_PointLights[i].m_LightCol.x, scene::m_PointLights[i].m_LightCol.y, scene::m_PointLights[i].m_LightCol.z);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].linear"), linear);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].quadratic"), quadratic);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].intensity"), scene::m_PointLights[i].intensity);
        }
        else
        {
            scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].position"), 0, 0, 0);
            scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].lightColor"), 0, 0, 0);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].linear"), 0);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].quadratic"), 0);
            scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].intensity"), 0);
        }

    }
    for (uint32_t i = scene::m_PointLights.size(); i < 25; i++)
    {
        scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].position"), 0, 0, 0);
        scene::GlobalShader->SetVec3f(("pointLights[" + std::to_string(i) + "].lightColor"), 0, 0, 0);
        scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].linear"), 0);
        scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].quadratic"), 0);
        scene::GlobalShader->SetFloat(("pointLights[" + std::to_string(i) + "].intensity"), 0);
    }

    for (uint32_t i = 0; i < scene::m_DirectionalLights.size(); i++)
    {
        if (scene::m_DirectionalLights[i].isWorking)
        {
            scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].direction", scene::m_DirectionalLights[i].m_Direction.x, scene::m_DirectionalLights[i].m_Direction.y, scene::m_DirectionalLights[i].m_Direction.z);
            scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].lightColor", scene::m_DirectionalLights[i].m_LightCol.x, scene::m_DirectionalLights[i].m_LightCol.y, scene::m_DirectionalLights[i].m_LightCol.z);
        }
        else
        {
            scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].direction", 0, 0, 0);
            scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
        }

    }
    for (uint32_t i = scene::m_DirectionalLights.size(); i < 25; i++)
    {
        scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].direction", 0, 0, 0);
        scene::GlobalShader->SetVec3f("dirLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
    }

    for (uint32_t i = 0; i < scene::m_SpotLights.size(); i++)
    {
        if (scene::m_SpotLights[i].isWorking)
        {
            scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].direction", scene::m_SpotLights[i].m_Direction.x, scene::m_SpotLights[i].m_Direction.y, scene::m_SpotLights[i].m_Direction.z);
            scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].lightColor", scene::m_SpotLights[i].m_LightCol.x, scene::m_SpotLights[i].m_LightCol.y, scene::m_SpotLights[i].m_LightCol.z);
            scene::GlobalShader->SetVec3f(("spotLights[" + std::to_string(i) + "].position"), scene::m_SpotLights[i].m_lightPos.x, scene::m_SpotLights[i].m_lightPos.y, scene::m_SpotLights[i].m_lightPos.z);

            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].cutOff"), glm::cos(glm::radians(scene::m_SpotLights[i].cutOff)));
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].outerCutOff"), glm::cos(glm::radians(scene::m_SpotLights[i].outerCutOff)));

            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].linear"), linear);
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].quadratic"), quadratic);
        }
        else
        {
            scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].direction", 0, 0, 0);
            scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].cutOff"), 0);
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].outerCutOff"), 0);
            scene::GlobalShader->SetVec3f(("spotLights[" + std::to_string(i) + "].position"), 0, 0, 0);
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].linear"), 0);
            scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].quadratic"), 0);
        }

    }
    for (uint32_t i = scene::m_SpotLights.size(); i < 10; i++)
    {
        scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].direction", 0, 0, 0);
        scene::GlobalShader->SetVec3f("spotLights[" + std::to_string(i) + "].lightColor", 0, 0, 0);
        scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].cutOff"), 0);
        scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].outerCutOff"), 0);
        scene::GlobalShader->SetVec3f(("spotLights[" + std::to_string(i) + "].position"), 0, 0, 0);
        scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].linear"), 0);
        scene::GlobalShader->SetFloat(("spotLights[" + std::to_string(i) + "].quadratic"), 0);
    }

    scene::GlobalShader->SetMatrix4f("view", camera->GetView());
    scene::GlobalShader->SetMatrix4f("projection", proj);
	//model view is set in the render function because it is different for each object
	

    scene::GlobalShader->SetVec3f("material.ambient", scene::ambient.x, scene::ambient.y, scene::ambient.z);

    for (auto mesh : scene::m_Models)
    {
        mesh.modelView = glm::mat4(1.0f);
        mesh.modelView = glm::translate(mesh.modelView, mesh.position);
        mesh.modelView = glm::rotate(mesh.modelView, glm::radians(mesh.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        mesh.modelView = glm::rotate(mesh.modelView, glm::radians(mesh.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        mesh.modelView = glm::rotate(mesh.modelView, glm::radians(mesh.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        mesh.modelView = glm::scale(mesh.modelView, mesh.scale);

        scene::GlobalShader->SetMatrix4f("model", mesh.modelView);
        scene::GlobalShader->SetFloat("material.shininess", mesh.m_material->shininess);

        Draw(mesh);
    }
}
