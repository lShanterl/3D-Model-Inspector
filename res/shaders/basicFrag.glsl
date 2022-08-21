#version 330 core 

layout(location = 0) out vec4 color;

in vec2 v_TexCoords;
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoords) * u_Color;
};