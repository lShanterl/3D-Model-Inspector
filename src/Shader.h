#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include "Commons.h"
#include "vendor/glm/glm.hpp"

class Shader
{
public:
	Shader(const std::string&& vertSource, const std::string&& fragSource);
	~Shader();

	const void Bind() const;
	const void Unbind() const;

	void SetVec4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetVec3f(const std::string& name, float v0, float v1, float v2);
	void SetVec2f(const std::string& name, float v0, float v1);
	void SetFloat(const std::string& name, float v0);

	void SetInt(const std::string& name, int v0);

	void SetMatrix4f(const std::string& name,const glm::mat4& matrix);

	int GetUniformLocation(const std::string& name);

private:
	void CreateShader(const std::string& vertexSource, const std::string& fragmentSource);

	int CompileShader(unsigned int type,const std::string& source);

	std::string ParseShader(const std::string& filepath);

	unsigned int m_Program;
};

#endif