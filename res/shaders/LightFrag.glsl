#version 330 core

struct Material
{
vec3 ambient;
sampler2D diffuse;
sampler2D specular;
float shininess;
};

out vec4 FragColor;

struct DirLight {
    vec3 direction;
	
    vec3 lightColor;
};

struct PointLight {
    vec3 position;
    
    float linear;
    float quadratic;
	
    vec3 lightColor;

    float intensity;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float linear;
    float quadratic;
    
    vec3 lightColor;
};

struct Scene
{
 int mPointLights;
};

#define NR_POINT_LIGHTS 8  


in vec3 FragPos;
in vec3 Normal;
in vec2 v_TexCoords;

uniform sampler2D u_Texture;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform Scene scene;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLights(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

    //ambient
    vec3 ambient = material.ambient * texture(material.diffuse, v_TexCoords).rgb;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = ambient;
    // phase 1: Directional lighting
        result += CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i <= scene.mPointLights; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    }
    // phase 3: Spot light
        result += CalcSpotLights(spotLight, norm, FragPos, viewDir);    

        

    FragColor = vec4(result, 1.0);
}
   

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 diffuse  = light.lightColor  * diff * texture(material.diffuse, v_TexCoords).rgb;
    vec3 specular = light.lightColor * spec * texture(material.specular, v_TexCoords).rgb;
    return (diffuse + specular) * 0;
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0f + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 diffuse  = light.lightColor  * diff * texture(material.diffuse, v_TexCoords).rgb;
    vec3 specular = light.lightColor * spec * texture(material.specular, v_TexCoords).rgb;

    diffuse  *= attenuation;
    specular *= attenuation;
    return (diffuse + specular)*light.intensity;
} 

vec3 CalcSpotLights(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.lightColor * texture(material.diffuse, v_TexCoords).rgb;

    //specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.lightColor * spec * texture(material.specular, v_TexCoords).rgb;

    //spotlight soft edges 
    float theta = dot(lightDir, normalize(-light.direction));

   float epsilon   = light.cutOff - light.outerCutOff;
   //float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

   float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / epsilon);
   
   diffuse  *= intensity;
   specular *= intensity;
   
   //attenuation 
   float distance = length(light.position - fragPos);
   float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));
   
   diffuse  *= attenuation;
   specular *= attenuation;
   
   vec3 result = (diffuse + specular) * 0;

   return result;
}