#pragma once

#ifndef COMMONS_H
#define COMMONS_H

#include <iostream>
#include <GL/glew.h>

#ifdef _DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

#endif