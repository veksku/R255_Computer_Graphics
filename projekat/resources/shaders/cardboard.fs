// #version 330 core
// out vec4 FragColor;
//
// struct Material {
//     sampler2D texture1;
//     sampler2D specular;
//     float shininess;
// };
//
// struct DirLight {
//     vec3 direction;
//
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };
//
// struct PointLight {
//     vec3 position;
//
//     float constant;
//     float linear;
//     float quadratic;
//
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };
//
// in vec3 FragPos;
// in vec3 Normal;
// in vec2 TexCoords;
//
// uniform vec3 viewPos;
// uniform Material material;
// uniform DirLight dirlight;
// uniform PointLight pointlight;
//
// void main()
// {
//     vec3 ambient = dirlight.ambient * texture(material.texture1, TexCoords).rgb;
//
//     vec3 norm = normalize(Normal);
//     vec3 lightDir = normalize(-dirlight.direction);
//     vec3 diffuse = dirlight.diffuse * 1.0 * texture(material.texture1, TexCoords).rgb;
//
//     // specular
//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     vec3 specular = dirlight.specular * spec * texture(material.specular, TexCoords).rgb;
//
//     vec3 result = ambient + diffuse + specular;
//     lightDir = normalize(pointlight.position - FragPos);
//
//     float distance = length(pointlight.position - FragPos);
//     float attenuation = 1.0 / (pointlight.constant + pointlight.linear * distance + pointlight.quadratic * (distance * distance));
//     ambient = pointlight.ambient * texture(material.texture1, TexCoords).rgb;
//     diffuse = pointlight.diffuse * 1.0 * texture(material.texture1, TexCoords).rgb;
//     specular = pointlight.specular * spec * texture(material.texture1, TexCoords).rgb;
//     ambient *= attenuation;
//     diffuse *= attenuation;
//     specular *= attenuation;
//     result =  ambient + diffuse + specular;
//     FragColor = vec4(result, 1.0);
// }

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoords);
}