// #version 330 core
// out vec4 FragColor;

// in vec2 TexCoords;

// uniform sampler2D texture_diffuse1;

// void main()
// {
  //   FragColor=texture(texture_diffuse1,TexCoords);
// }
#version 330 core
out vec4 FragColor;

void main()
{
  FragColor=vec4(1.,1.,0.,1.);
}