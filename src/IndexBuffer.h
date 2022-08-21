#pragma once

#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <GL/glew.h>
#include "Commons.h"
#include "Vertex.h"

class IndexBuffer
{
public:
	IndexBuffer(unsigned int* data, unsigned int count);
	//IndexBuffer(Vertex* vertices, unsigned int count);
	~IndexBuffer();

	const unsigned int& GetCount() const { return m_Count; }
	
	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

};

#endif