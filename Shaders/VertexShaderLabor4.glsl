#version 410
uniform mat4 matrix;
in vec3 position;
in vec2 uv;
in vec3 normal;

out vec3 normalVectorForFragmentShader;
out vec3 fragPos;
out vec2 UV;

void main()
{
    gl_Position = matrix * vec4(position, 1.0);
       
    UV = uv;
}
