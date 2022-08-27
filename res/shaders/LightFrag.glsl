#version 330 core

struct Material
{
vec3 ambient;
sampler2D diffuse;
sampler2D specular;
float shininess;
};

uniform Material material;

out vec4 FragColor;
  
uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 viewPos;

uniform sampler2D u_Texture;

in vec2 v_TexCoords;

in vec3 Normal; 
in vec3 FragPos; 

void main()
{ 
    //ambient
    vec3 ambient = material.ambient * texture(material.diffuse, v_TexCoords).rgb;
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightDir - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * texture(material.diffuse, v_TexCoords).rgb;

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * spec * texture(material.specular, v_TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}