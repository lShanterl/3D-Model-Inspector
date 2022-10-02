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
#define NR_DIR_LIGHTS 4
#define NR_SPOT_LIGHTS 4


in vec3 fragPos;
in vec3 Normal;
in vec2 v_TexCoords;

uniform sampler2D u_Texture;

uniform vec3 viewPos;
uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
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
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = ambient;
    // phase 1: Directional lighting
    for(int i = 0; i <= NR_DIR_LIGHTS; i++)
        result += CalcDirLight(dirLights[i], norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i <= NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);    
    }
    // phase 3: Spot light
    for(int i = 0; i <= NR_SPOT_LIGHTS; i++)
    {
      result += CalcSpotLights(spotLights[i], norm, fragPos, viewDir);    
    }

        

    FragColor = vec4(result, 1.0);

    // apply gamma correction
    //float gamma = 2.2;
    //FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
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
    return (diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
    //vec3 reflectDir = reflect(-lightDir, normal);
	
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	
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
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.lightColor * texture(material.diffuse, v_TexCoords).rgb;

    //specular
    //vec3 reflectDir = reflect(-lightDir, normal);  
	
	vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.lightColor * spec * texture(material.specular, v_TexCoords).rgb;

    //spotlight soft edges 
    float theta = dot(lightDir, normalize(-light.direction));

   float epsilon   = light.cutOff - light.outerCutOff;
   //float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

   float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / epsilon);
   
   diffuse  *= intensity;
   specular *= intensity;
   
   //attenuation 
   float distance = length(light.position - fragPos);
   float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));
   
   diffuse  *= attenuation;
   specular *= attenuation;
   
   vec3 result = (diffuse + specular);

   return result;
}