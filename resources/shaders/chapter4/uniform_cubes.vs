#version 330 core
layout(std140)uniform Matrices
{
  mat4 projection;
  mat4 view;
};

layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
uniform mat4 model;

out vec3 Normal;
out vec3 Position;

void main()
{
  Normal=mat3(transpose(inverse(model)))*aNormal;
  Position=vec3(model*vec4(aPos,1.));
  gl_Position=projection*view*model*vec4(aPos,1.);
}