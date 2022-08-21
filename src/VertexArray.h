#pragma once
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexBufferLayout.h"

#include <GL/glew.h>
#include "Commons.h"

class VertexArray
{
public:
	VertexArray(const void* data, unsigned int size,const VertexBufferLayout& layout);
	//VertexArray();

	~VertexArray();

	void Bind() const;

	void Unbind() const;

	unsigned int GetCount() const;
	unsigned int& GetVBO();

private:
	unsigned int m_count;
	unsigned int VAO, VBO;
};

#endif