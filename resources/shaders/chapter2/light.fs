#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform Material material;
uniform vec3 viewPos;

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);

void main()
{
    // 属性
    vec3 norm=normalize(Normal);
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 result=vec3(0.);
    // 第一阶段：定向光照
    result+=CalcDirLight(dirLight,norm,viewDir);
    // 第二阶段：点光源
    for(int i=0;i<NR_POINT_LIGHTS;i++){
        result+=CalcPointLight(pointLights[i],norm,FragPos,viewDir);
    }
    // 第三阶段：聚光
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    
    FragColor=vec4(result,1.);
}

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
    vec3 lightDir=normalize(-light.direction);
    // 漫反射着色
    float diff=max(dot(normal,lightDir),0.);
    // 镜面光着色
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.),material.shininess);
    // 合并结果
    vec3 ambient=light.ambient*vec3(texture(material.diffuse,TexCoords));
    vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));
    vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoords));
    return(ambient+diffuse+specular);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
    vec3 lightDir=normalize(light.position-fragPos);
    // 漫反射着色
    float diff=max(dot(normal,lightDir),0.);
    // 镜面光着色
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.),material.shininess);
    // 衰减
    float dis=length(light.position-fragPos);
    float attenuation=1./(light.constant+light.linear*dis+light.quadratic*(dis*dis));
    attenuation*=2;
    // 合并结果
    vec3 ambient=light.ambient*vec3(texture(material.diffuse,TexCoords));
    vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));
    vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoords));
    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;
    return(ambient+diffuse+specular);
}