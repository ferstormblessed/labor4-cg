#version 410  

in vec2 UV;
uniform sampler2D myTextureSampler;

out vec4 FragColor;

void main()  
{  
    vec4 texColor = texture(myTextureSampler, UV);
    FragColor = texColor;
}
