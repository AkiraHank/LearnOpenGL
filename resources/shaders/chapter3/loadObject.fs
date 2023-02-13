#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform vec3 viewPos;
uniform vec3 pointLightPosition;

vec3 CalcPointLight(vec3 normal,vec3 viewDir);
void main()
{
  // TODO: 这里的法线贴图该怎么用？
  vec3 normal=texture(texture_normal1,TexCoords).rgb;
  vec3 norm=normalize(normal*2.-1.);
  
  vec3 viewDir=normalize(viewPos-FragPos);
  
  vec3 result=CalcPointLight(norm,viewDir);
  
  FragColor=vec4(result,1.);
}

vec3 CalcPointLight(vec3 normal,vec3 viewDir)
{
  vec3 lightDir=normalize(pointLightPosition-FragPos);
  // 漫反射着色
  float diff=max(dot(normal,lightDir),0.);
  // 镜面光着色
  vec3 reflectDir=reflect(-lightDir,normal);
  float spec=pow(max(dot(viewDir,reflectDir),0.),32.);
  // 衰减
  float dis=length(pointLightPosition-FragPos);
  float attenuation=1./(1.+.09*dis+.032*(dis*dis));
  attenuation*=3;
  // 合并结果
  vec3 diffuse=texture(texture_diffuse1,TexCoords).rgb;
  vec3 specular=spec*vec3(.2);
  diffuse*=attenuation;
  specular*=attenuation;
  return(diffuse+specular);
}