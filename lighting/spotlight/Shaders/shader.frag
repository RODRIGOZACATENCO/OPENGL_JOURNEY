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
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;//missingx
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

};

uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;

uniform vec3 viewPos;

void main()
{
    //combines box and smiley face textures
    vec3 mixTexture=mix(texture(material.diffuse1,texCoord),texture(material.diffuse2,texCoord),0.2).rgb;
    // ambient
    vec3 ambient = light.ambient * mixTexture;



    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position-FragPos);//vector from the fragment to light position
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff *mixTexture;

    //specular
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular=spec* (texture(material.specular,texCoord).rgb)*light.specular;

    //soft edges for spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    //attenuation
    float distance=length(light.position-FragPos);//distance from the light source to the fragment
    float attenuation=1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;


    vec3 result=ambient+diffuse+specular;


    FragColor = vec4(result, 1.0);
}