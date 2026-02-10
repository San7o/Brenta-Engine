#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int atlasSize = 4;
uniform int atlasIndex = 0;

out vec3 Normal;
out vec3 FragPos; // position of the fragment in world space
out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // Use this the normal matrix (tranpose of the inverse of the model)
    // when we have non-uniform scaling
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Atlas offset
    vec2 offset = vec2((1.0 / atlasSize) * (atlasIndex % atlasSize), 0.0);
    TexCoords = aTexCoords + offset;
}
