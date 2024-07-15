#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Light attenuation
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

out vec4 FragColor; 

void main()
{
    // ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position);
    float diff = max(dot(norm, lightDir), 0.0); // max to avoid negative values
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));

    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

    // combine results
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
