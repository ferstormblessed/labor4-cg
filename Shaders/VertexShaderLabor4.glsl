#version 410
uniform mat4 matrix;
in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 normalVectorForFragmentShader;
out vec3 fragPos;
out vec2 UV;

void main()
{
    gl_Position = matrix * vec4(position, 1.0);
    fragPos = (matrix * vec4(position, 1.0)).xyz;
    normalVectorForFragmentShader = (matrix * vec4(normal, 1.0)).xyz;
    UV = uv;
}
