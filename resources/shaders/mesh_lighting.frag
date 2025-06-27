// mesh_lighting.frag
#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform bool textured;
uniform sampler2D textureSampler;
uniform vec4 color;

// View
uniform vec3 viewPos;
uniform float shininess;

// Directional light
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

// Point light
struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointLight;

// Spot light
struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float cutOff;
    float outerCutOff;
};
uniform SpotLight spotLight;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // === Directional Light ===
    vec3 dirLightDir = normalize(-lightDir);
    float diff = max(dot(norm, dirLightDir), 0.0);
    vec3 reflectDir = reflect(-dirLightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient = ambientColor;
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;

    vec3 result = ambient + diffuse + specular;

    // === Point Light ===
    vec3 pointDir = normalize(pointLight.position - FragPos);
    float diffP = max(dot(norm, pointDir), 0.0);
    vec3 reflectP = reflect(-pointDir, norm);
    float specP = pow(max(dot(viewDir, reflectP), 0.0), shininess);
    float distP = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distP + pointLight.quadratic * (distP * distP));

    vec3 diffuseP = diffP * pointLight.color * attenuation;
    vec3 specularP = specP * pointLight.color * attenuation;

    result += diffuseP + specularP;

    // === Spot Light ===
    vec3 spotDir = normalize(spotLight.position - FragPos);
    float theta = dot(normalize(-spotDir), normalize(spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

    float diffS = max(dot(norm, spotDir), 0.0);
    vec3 reflectS = reflect(-spotDir, norm);
    float specS = pow(max(dot(viewDir, reflectS), 0.0), shininess);

    vec3 diffuseS = diffS * spotLight.color * intensity;
    vec3 specularS = specS * spotLight.color * intensity;

    result += diffuseS + specularS;

    // === Final color ===
    vec3 baseColor = textured ? texture(textureSampler, TexCoords).rgb : color.rgb;
    vec3 finalColor = result * baseColor;

    // Optional: gamma correction
    finalColor = pow(finalColor, vec3(1.0 / 2.2)); // comment out if you don't want gamma

    FragColor = vec4(finalColor, color.a);
}
