#version 330 core
in vec3 position;
in vec3 normal;
uniform mat4 matrix;

out vec3 fragNormal;

void main()
{
    gl_Position = matrix * vec4(position, 1.0);
    fragNormal = normal;
}
