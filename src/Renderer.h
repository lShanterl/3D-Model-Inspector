#pragma once


#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include "Model.h"

class Renderer
{
public:
	void Clear();
	void SetClearColor(float r, float g, float b, float a);
	void Draw(Model& mesh);
private:
	
};

#endif