#include "Renderer.h"
#include <iostream>

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