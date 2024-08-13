#version 330 core

// Structs

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
uniform Material material;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float dir_strength;
};
uniform DirLight dirLight;
uniform bool useDirLight = false; // Set this to true to enable directional light

struct PointLight {
    vec3 position;
    float point_strength;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Light attenuation
    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int nPointLights = 0; // Set this to the number of point lights you have

uniform float transparency = 1.0;

// inputs
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// Uniforms
uniform vec3 viewPos;

// Outputs
out vec4 FragColor; 

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec4 textColor = texture(material.texture_diffuse1, TexCoords);
    if (textColor.a < 0.1) discard;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    // Directional light
    if (useDirLight) {
        result = CalcDirLight(dirLight, norm, viewDir);
    }
    else {
        // If we don't have a directional light, just use the texture
        result = vec3(textColor);
    }

    // Point lights
    if (nPointLights > 0) {
        for (int i = 0; i < NR_POINT_LIGHTS; i++) {
            if (i >= nPointLights) break;
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
        }
    }

    // Output result
    FragColor = vec4(result, transparency);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) 
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess );

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    return (ambient + diffuse + specular) * light.dir_strength;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) * light.point_strength;
}
