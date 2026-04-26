#version 330 core

out vec4 FragColor;

struct Material {
sampler2D diffuse1;
sampler2D diffuse2;
sampler2D specular;
float shininess;
};
uniform Material material;

struct Light{
vec3 direction;//we use the direction the light is coming from
vec3 ambient;
vec3 diffuse;
vec3 specular;
};
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;

uniform vec3 viewPos;

void main()
{
    vec3 mixTexture=mix(texture(material.diffuse1,texCoord),texture(material.diffuse2,texCoord),0.2).rgb;

    // ambient
     vec3 ambient = light.ambient * mixTexture;


    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = light.diffuse * diff *mixTexture;


    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular=spec* (texture(material.specular,texCoord).rgb)*light.specular;


    vec3 result = ambient+diffuse+specular;
    FragColor = vec4(result, 1.0);
}