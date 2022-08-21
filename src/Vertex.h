#pragma once

#ifndef VERTEX_H
#define VERTEX_H
#include "Commons.h"
#include "vendor/glm/glm.hpp"
struct Vertex
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
};


#endif // !VERTEX_H
