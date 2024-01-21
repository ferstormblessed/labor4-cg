#version 410  

in vec3 normalVectorForFragmentShader;
in vec3 fragPos;
in vec2 UV;

uniform float ambientFactor;
uniform float diffuseFactor;
uniform float shininess;
uniform float specFactor;
uniform sampler2D myTextureSampler;

out vec4 FragColor;

vec3 lightPos = vec3(0, 0, 1);
vec3 cameraPos = vec3(0, 0, 1);

void main()  
{  
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 refectDir = reflect(-lightDir, normalVectorForFragmentShader);
    float aFactor = ambientFactor;
    float dFactor = (max(dot(normalVectorForFragmentShader, lightDir), 0.0)) * diffuseFactor;
    float sFactor = pow(max(dot(viewDir, refectDir), 0.0), shininess) * specFactor;
    vec4 texColor = texture(myTextureSampler, UV);
    FragColor = texColor * (aFactor + dFactor + sFactor);
}
