#pragma once

#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <vector>
#include <GL/glew.h>

struct VertexBufferElement
{
	VertexBufferElement(unsigned int _type, unsigned int _count, unsigned int _size, bool _normalized)
		:type(_type),count(_count),size(_size), normalized(_normalized) {}
	unsigned int type;
	unsigned int count;
	unsigned int size;
	bool normalized;
};

class VertexBufferLayout
{
public:
	template<typename T>
	void Push(unsigned int count)
	{
		unsigned int type, size;
		if constexpr (std::same_as<T, float>)
		{
			type = GL_FLOAT;
			size = sizeof(float);
		}
		else if constexpr (std::same_as<T, unsigned int>)
		{
			type = GL_UNSIGNED_INT;
			size = sizeof(unsigned int);
		}
		else if constexpr (std::same_as<T, unsigned char>)
		{
			type = GL_UNSIGNED_BYTE;
			size = sizeof(unsigned char);
		}
		m_Elements.push_back({ type, count,size, GL_FALSE });
		m_Stride += sizeof(T) * count;
	}
	const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	const unsigned int& GetStride() const { return m_Stride; }
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride = 0;
};

#endif