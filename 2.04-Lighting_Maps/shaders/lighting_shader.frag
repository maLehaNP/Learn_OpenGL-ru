#version 330 core
out vec4 color;

in vec3 Normal;
in vec3 FragPos;

in vec2 TexCoords;

struct Material {
  //vec3 ambient;
  //vec3 diffuse;

  sampler2D diffuse;

  //vec3 specular;
  sampler2D specular;
  float shininess;  // Радиус блика
};

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;


void main()
{
  // Ambient component
  //vec3 ambient = light.ambient * material.ambient;
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  // Diffuse component
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  //vec3 diffuse = light.diffuse * (diff * material.diffuse);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

  // Specular component
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  //vec3 specular = light.specular * (spec * material.specular) ;
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

  vec3 result = ambient + diffuse + specular;
  color = vec4(result, 1.0f);
}

