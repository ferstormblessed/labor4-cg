#version 330 core
in vec3 fragNormal;

out vec4 FragColor;

void main()
{
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(normalize(fragNormal), lightDir), 0.0);
    vec3 color = vec3(1.0, 0.5, 0.2); // Diffuse color
    FragColor = vec4(color * diff, 1.0);
}
