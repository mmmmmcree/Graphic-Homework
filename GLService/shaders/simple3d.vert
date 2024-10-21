#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 viewPos;

uniform mat4 model;
layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};
void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    TexCoords = tex_coord;
    viewPos = camera_position;
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;  
    // TexCoords = tex_coord;
    
    // gl_Position = projection * view * vec4(FragPos, 1.0);
}