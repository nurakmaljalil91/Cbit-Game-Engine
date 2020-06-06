#version 330 core

in vec2 TexCoord;
// in vec3 color;
// uniform vec4 vertColor;
out vec4 frag_color;
uniform sampler2D myTexture1; // first texture
uniform sampler2D myTexture2; // second texture

void main()
{
    frag_color = mix(texture(myTexture1, TexCoord), texture(myTexture2, TexCoord), 0.5);
};