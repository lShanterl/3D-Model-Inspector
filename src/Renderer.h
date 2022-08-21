#pragma once


#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

class Renderer
{
public:
	void Clear();
	void SetClearColor(float r, float g, float b, float a);
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
private:
	
};

#endif