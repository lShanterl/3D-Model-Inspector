#version 330 core 

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 aNormal;

out vec2 v_TexCoords;
out vec3 Normal;
out vec3 fragPos;

uniform mat4 u_MVP;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverseModel;

void main()
{
	gl_Position =  projection * view * model  * position;
	 fragPos = vec3(model * position);
	v_TexCoords = texCoord;
	Normal = mat3(transpose(inverse(model))) * aNormal;  
};