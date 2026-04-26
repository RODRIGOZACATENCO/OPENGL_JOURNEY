#version 330 core

out vec4 FragColor;

struct Material {
sampler2D diffuse1;
sampler2D diffuse2;
sampler2D specular;
float shininess;
};
uniform Material material;

//directional light, its global, covers the entire scene, like the sun
struct DirLight{
    vec3 direction;//pointing towards fragments
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

//point light, like a lamp, has attenuation over distance
struct PointLight{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

};

struct SpotLight{
    vec3 direction;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

};
#define NUM_POINT_LIGHTS 4

uniform DirLight dirlight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];//for lamps on the scene
uniform SpotLight spotLight;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;

uniform vec3 viewPos;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

vec3 CalcPointLight(PointLight pointlight,vec3 normal,vec3 FragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotlight,vec3 normal, vec3 FragPos,vec3 viewDir);
void main()
{
    vec3 norm=normalize(Normal);
    vec3 viewDir=normalize(viewPos-FragPos);

    vec3 resultDir=CalcDirLight(dirlight,norm,viewDir);
    vec3 resultPoint=vec3(0.0f);
    for(int i=0; i<NUM_POINT_LIGHTS; i++){
        resultPoint+= CalcPointLight(pointLights[i],norm,FragPos,viewDir);
    }
    vec3 resultSpot= CalcSpotLight(spotLight,norm,FragPos,viewDir);

    vec3 result=+resultPoint+resultSpot;
    FragColor = vec4(result, 1.0);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    //DIFFUSE
    vec3 lightDir=normalize(-light.direction);//invert to point outwards from fragments

    float difference=max(dot(lightDir,normal),0.0f);//how much the angle between fragment normals and the lights direction
    //1 for small angles, 0 for high angles

    //SPECULAR
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

    vec3 mixTexture=mix(texture(material.diffuse1,texCoord),texture(material.diffuse2,texCoord),0.2).rgb;

    vec3 ambient=mixTexture*light.ambient;
    vec3 diffuse=mixTexture*light.diffuse*difference;
    vec3 specular=mixTexture*light.specular*spec;

    return (ambient+diffuse+specular);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 FragPos,vec3 viewDir){

    vec3 lightDir=normalize(light.position-FragPos);
    //DIFFUSE
    float difference=max(dot(lightDir,normal),0.0f);

    //SPECULAR
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

    //attenuation for distances
    float distance=length(light.position-FragPos);

    float attenuation=1.0f/(light.constant+light.linear*distance+light.quadratic*distance*distance);

    vec3 mixTexture=mix(texture(material.diffuse1,texCoord),texture(material.diffuse2,texCoord),0.2).rgb;

    vec3 ambient=mixTexture*light.ambient;
    vec3 diffuse=mixTexture*light.diffuse*difference;
    vec3 specular=mixTexture*light.specular*spec;

    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;

    return (ambient+diffuse+specular);
}

vec3 CalcSpotLight(SpotLight light,vec3 normal, vec3 FragPos,vec3 viewDir){
    vec3 mixTexture=mix(texture(material.diffuse1,texCoord),texture(material.diffuse2,texCoord),0.2).rgb;

    vec3 norm=normalize(normal);

    //diffuse
    vec3 lightDir=normalize(light.position-FragPos);
    float diff=max(dot(norm,lightDir),0.0f);

    //specular
    vec3 reflection=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflection),0.0),material.shininess);

    vec3 ambient=light.ambient*mixTexture;
    vec3 diffuse=light.diffuse*mixTexture*diff;
    vec3 specular=light.specular*mixTexture*spec;
    //atenuation for distances

    float distance=length(light.position-FragPos);
    float attenuation=1.0f/(light.constant+light.linear*distance+light.quadratic*distance*distance);
    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;

    //intensity of the light cone, lower intensity outside the cones range
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    ambient*=intensity;
    diffuse*=intensity;
    specular*=intensity;





    return (ambient+diffuse+specular);
}