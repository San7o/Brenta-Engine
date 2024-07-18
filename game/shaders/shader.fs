#version 330 core

// Structs

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
uniform Material material;

// inputs
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// Uniforms
uniform vec3 viewPos;

// Outputs
out vec4 FragColor; 

void main()
{
    // Output result
    FragColor = texture(material.texture_diffuse1, TexCoords);
}

