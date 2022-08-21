#include "VertexArray.h"
#include "Vertex.h"
VertexArray::VertexArray(const void* data, unsigned int size,const VertexBufferLayout& layout)
	:m_count(size/sizeof(Vertex))
{
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glBindVertexArray(VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));


	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(),(const void*)offset));
		offset += element.count * element.size;

	}
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &VAO));
	GLCall(glDeleteBuffers(1, &VBO));
}

void VertexArray::Bind() const 
{
	GLCall(glBindVertexArray(VAO));
	
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

unsigned int VertexArray::GetCount() const
{
	return m_count;
}
unsigned int& VertexArray::GetVBO()
{

	return VBO;
}


