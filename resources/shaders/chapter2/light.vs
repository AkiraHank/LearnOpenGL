#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat3 model_it;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    // vec3 normal;
    // if (aPos.x == 0.5) {
    //     normal = vec3(1, 0, 0);
    // } else if (aPos.x == -0.5) {
    //     normal = vec3(-1, 0, 0);
    // } else if (aPos.y == 0.5) {
    //     normal = vec3(0, 1, 0);
    // } else if (aPos.y == -0.5) {
    //     normal = vec3(0, -1, 0);
    // } else if (aPos.z == 0.5) {
    //     normal = vec3(0, 0, 1);
    // } else if (aPos.z == -0.5) {
    //     normal = vec3(0, 0, -1);
    // }
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = model_it * aNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}