#version 330 core

in  vec2 TexCoords;
out vec4 FragColor;

uniform bool    textured;
uniform sampler2D textureSampler;
uniform vec4    color;


void main() {
    if (textured) {
        // sample the texture, then tint by `color`
        FragColor = texture(textureSampler, TexCoords) * color;
    } else {
        // solid‐color mesh
        FragColor = color;
    }
}