#version 330 core
out vec4 color;

in vec3 Normal;
in vec3 FragPos;

//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;  // Радиус блика
};

uniform Material material;

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Light light;


void main()
{
  // Ambient component
  //float ambientStrength = 0.1f;
  //vec3 ambient = ambientStrength * lightColor;
  vec3 ambient = light.ambient * material.ambient;

  // Diffuse component
  vec3 norm = normalize(Normal);
  //vec3 lightDir = normalize(lightPos - FragPos);
  vec3 lightDir = normalize(light.position - FragPos);
  // Величина воздействия света на фрагмент
  float diff = max(dot(norm, lightDir), 0.0);
  //vec3 diffuse = diff * lightColor;
  vec3 diffuse = light.diffuse * (diff * material.diffuse);

  // Specular component
  float specularStrength = 0.7f;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
  //vec3 specular = specularStrength * spec * lightColor;
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * material.specular) ;

  //vec3 result = (ambient + diffuse + specular) * objectColor;
  vec3 result = ambient + diffuse + specular;
  color = vec4(result, 1.0f);
}

