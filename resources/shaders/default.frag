#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture;

// Light properties
uniform vec3 u_LightDirection;
uniform vec3 u_LightColor;
uniform vec3 u_AmbientColor;

// Material properties
uniform vec3 u_ViewPos;
uniform float u_Shininess;

void main() {
    // Normalize vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-u_LightDirection); // opposite direction of light

    // Ambient
    vec3 ambient = u_AmbientColor;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // Specular
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
    vec3 specular = spec * u_LightColor;

    vec3 result = (ambient + diffuse + specular) * texture(u_Texture, TexCoord).rgb;
    FragColor = vec4(result, 1.0);
}
