#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform bool textured;
uniform sampler2D textureSampler;
uniform vec4 color;

// Lighting uniforms
uniform vec3 lightDir;     // e.g., vec3(-1.0, -1.0, -1.0)
uniform vec3 lightColor;   // e.g., vec3(1.0, 1.0, 1.0)
uniform vec3 viewPos;      // camera position
uniform vec3 ambientColor; // e.g., vec3(0.2, 0.2, 0.2)
uniform float shininess;   // e.g., 32.0

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-lightDir); // toward the light
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, norm);

    // Ambient
    vec3 ambient = ambientColor;

    // Diffuse
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
    vec3 specular = spec * lightColor;

    vec3 lighting = ambient + diffuse + specular;
    vec3 baseColor = textured ? texture(textureSampler, TexCoords).rgb : color.rgb;

    FragColor = vec4(baseColor * lighting, color.a);
}
