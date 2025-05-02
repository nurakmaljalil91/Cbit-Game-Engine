#version 330 core

in  vec2 TexCoord;
out vec4 FragColor;

// if you want to sample a texture:
// bind your GL_TEXTURE_2D to texture unit 0 before drawing,
// then call shader.setInt("uTexture", 0);
uniform sampler2D uTexture;

// if you just want a solid color, you can replace the body with FragColor = vec4(1.0,0.5,0.2,1.0);
void main() {
    FragColor = texture(uTexture, TexCoord);
}
