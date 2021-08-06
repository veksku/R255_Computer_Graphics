#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture1diffuse;
    sampler2D texture2diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform Material material;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights
    result += CalcPointLight(pointLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.35).rgb;
    vec3 diffuse = light.diffuse * diff * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.35).rgb;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.45).rgb;
    vec3 diffuse = light.diffuse * diff * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.45).rgb;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}


// #version 330 core
// out vec4 FragColor;
//
// struct Material {
//     sampler2D texture1diffuse;
//     sampler2D texture2diffuse;
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
// uniform DirLight dirLight;
// uniform PointLight pointLight;
//
// void main()
// {
//     // ambient
//     vec3 ambient = dirLight.ambient * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.35).rgb;
//
//     vec3 norm = normalize(Normal);
//     // diffuse
//     vec3 lightDir = normalize(-dirLight.direction);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = dirLight.diffuse * diff * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.35).rgb;
//
//     // specular
//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     vec3 specular = dirLight.specular * spec * texture(material.specular, TexCoords).rgb;
//
//     vec3 result = ambient + diffuse + specular;
//     // result trenutno sadrzi samo DirLight----------------------------------------
//
//     //novi lightDir u odnosu na poziciju izvora pointlighta
//     lightDir = normalize(pointLight.position - FragPos);
//     //novi diff jer se koristi drugaciji lightdir
//     diff = max(dot(norm, lightDir), 0.0);
//     // spekularno je vec definisano
//
//     // opadanje jacine svetlosti
//     float distance = length(pointLight.position - FragPos);
//     float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
//     // combine results
//     ambient = pointLight.ambient * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.45).rgb;
//     diffuse = pointLight.diffuse * diff * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.45).rgb;
//     specular = pointLight.specular * spec * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.45).rgb;
//     ambient *= attenuation;
//     diffuse *= attenuation;
//     specular *= attenuation;
//     result = result + ambient + diffuse + specular;
//     FragColor = vec4(result, 1.0);
// }