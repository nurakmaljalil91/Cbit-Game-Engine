#version 330 core // version tag OpenGL 3.3



layout (location = 0) in vec3 pos; 
// layout (location = 0) exist during OpenGL 3.3
// specifies the location for the attributes
// glVertexAttributePtr(location)
layout (location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 TexCoord;

// out vec3 color;
uniform vec2 posOffsets;

void main()
{
    gl_Position = projection * view * model * vec4(pos,1.f);
    TexCoord = texCoord;
};