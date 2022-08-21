#include "Shader.h"

Shader::Shader(const std::string&& vertSource, const std::string&& fragSource)
{
	CreateShader(vertSource, fragSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_Program);
}

const void Shader::Bind() const
{
	glUseProgram(m_Program);
}

const void Shader::Unbind() const
{
	glUseProgram(0);
}


void Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	m_Program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, ParseShader(vertexSource));
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, ParseShader(fragmentSource));

	GLCall(glAttachShader(m_Program, vs));
	GLCall(glAttachShader(m_Program, fs));
	GLCall(glLinkProgram(m_Program));
	GLCall(glValidateProgram(m_Program));

	GLCall(glUseProgram(m_Program));
	glDeleteShader(vs);
	glDeleteShader(fs);
}

int Shader::CompileShader(unsigned int type,const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = &source[0];
	glShaderSource(id, 1, &src, nullptr);
	GLCall(glCompileShader(id));
	int result;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? " vertex " : " fragment ") << "shader!\n";
		std::cout << message << std::endl;

		glDeleteShader(id);
		delete[] message;
		
	}

	return id;
}

std::string Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string shaderSource;

	std::string line;
	while (getline(stream, line))
	{
		shaderSource += line + "\n";
	}
	return shaderSource;
}

void Shader::SetVec4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetVec3f(const std::string& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));

}

void Shader::SetVec2f(const std::string& name, float v0, float v1)
{
	GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetFloat(const std::string& name, float v0)
{
	GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetInt(const std::string& name, int v0)
{
	GLCall(glUniform1i(GetUniformLocation(name), v0));

}

void Shader::SetMatrix4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE,&matrix[0][0]);
}


int Shader::GetUniformLocation(const std::string& name)
{
	int pos = glGetUniformLocation(m_Program, name.c_str());
	if (pos == GL_INVALID_VALUE)
	{
		std::cout << "Warning: position " << name << " does not exist \n";
	}
	return pos;

}