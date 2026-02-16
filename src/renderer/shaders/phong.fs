#version 330 core

//
// Phong uses two textures, diffuse and specular, and an ambient
// color to light the object.
//

// Structs and uniforms

struct Material
{
  // These values as specific to phong
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  float     shininess;
};
uniform Material material;

struct DirLight
{
  vec3  direction;
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;
  float strength;
};
uniform DirLight dir_light;
uniform bool     use_dir_light = false; // Set this to true to enable directional light

struct PointLight
{
  vec3  position;
  float strength;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  // Light attenuation
  float constant;
  float linear;
  float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight point_lights[NR_POINT_LIGHTS];
uniform int n_point_lights = 0; // Set this to the number of point lights you have

uniform vec3 view_pos;
uniform float transparency = 1.0;

// inputs
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// Outputs
out vec4 FragColor; 

// Function prototypes
vec3 calc_dir_light(DirLight light, vec3 normal, vec3 viewDir);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
  vec4 textColor = texture(material.texture_diffuse1, TexCoords);
  if (textColor.a < 0.1) discard;

  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(view_pos - FragPos);

  vec3 result = vec3(0.0);

  // Directional light
  if (use_dir_light)
  {
    result = calc_dir_light(dir_light, norm, viewDir);
  }
  else
  {
    // If we don't have a directional light, just use the texture
    result = vec3(textColor);
  }

  // Point lights
  if (n_point_lights > 0)
  {
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
      if (i >= n_point_lights) break;
      result += calc_point_light(point_lights[i], norm, FragPos, viewDir);
    }
  }

  // Output result
  FragColor = vec4(result, transparency);
}

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 viewDir) 
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
  return (ambient + diffuse + specular) * light.strength;
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
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
  return (ambient + diffuse + specular) * light.strength;
}
