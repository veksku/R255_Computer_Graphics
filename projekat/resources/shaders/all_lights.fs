// #version 330 core
// out vec4 FragColor;
//
// // struct PointLight {
// //     vec3 position;
// //
// //     vec3 specular;
// //     vec3 diffuse;
// //     vec3 ambient;
// //
// //     float constant;
// //     float linear;
// //     float quadratic;
// // };
//
// struct Light {
//     vec3 position;
//
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
//
//     float constant;
//     float linear;
//     float quadratic;
// };
//
// struct Material {
//     sampler2D texture1;
//     sampler2D texture2;
//     vec3 specular;
//     float shininess;
// };
//
// in vec3 FragPos;
// in vec3 Normal;
// in vec2 TexCoords;
//
// uniform vec3 viewPos;
// uniform Material material;
// uniform Light light;
//
// //uniform PointLight pointLight;
// //uniform Material material;
// //uniform vec3 viewPosition;
//
// // calculates the color when using a point light.
// // vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// // {
// //     vec3 lightDir = normalize(light.position - fragPos);
// //     // diffuse shading
// //     float diff = max(dot(normal, lightDir), 0.0);
// //     // specular shading
// //     vec3 reflectDir = reflect(-lightDir, normal);
// //     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
// //     // attenuation
// //     float distance = length(light.position - fragPos);
// //     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
// //     // combine results
// //     vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
// //     vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
// //     vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords).xxx);
// //     ambient *= attenuation;
// //     diffuse *= attenuation;
// //     specular *= attenuation;
// //     return (ambient + diffuse + specular);
// // }
//
// void main()
// {
// //     vec3 normal = normalize(Normal);
// //     vec3 viewDir = normalize(viewPosition - FragPos);
// //     vec3 result = CalcPointLight(pointLight, normal, FragPos, viewDir);
// //     FragColor = vec4(result, 1.0);
//     // ambient
//     vec3 ambient = light.ambient * mix(texture(material.texture1, TexCoords), texture(material.texture2, TexCoords), 0.35).rgb;
//     // diffuse
//     vec3 norm = normalize(Normal);
//     vec3 lightDir = normalize(light.position - FragPos);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = light.diffuse * diff * mix(texture(material.texture1, TexCoords), texture(material.texture2, TexCoords), 0.35).rgb;   //mozda ovde metnes mix(texture)
//
//     // specular
//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     vec3 specular = light.specular * (spec * material.specular);
//
//     float distance = length(light.position - FragPos);
//     float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * distance * distance);
//
//     //ambient *= attenuation;
//     diffuse *= attenuation;
//     specular *= attenuation;
//
//     vec3 result = ambient + diffuse + specular;
//     FragColor = vec4(result, 1.0);
// }


//basic lightning
// #version 330 core
// out vec4 FragColor;
//
// in vec3 Normal;
// in vec3 FragPos;
//
// uniform vec3 lightPos;
// uniform vec3 lightColor;
// uniform vec3 objectColor;
//
// void main()
// {
//     // ambient
//     float ambientStrength = 0.1;
//     vec3 ambient = ambientStrength * lightColor;
//
//     // diffuse
//     vec3 norm = normalize(Normal);
//     vec3 lightDir = normalize(lightPos - FragPos);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = diff * lightColor;
//
//     vec3 result = (ambient + diffuse) * objectColor;
//     FragColor = vec4(result, 1.0);
// }

// SA SAMO DIRECTIONAL LIGHTOM
#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture1diffuse;
    sampler2D texture2diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    //vec3 position;
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
uniform Material material;
uniform DirLight dirlight;
uniform PointLight pointlight;

void main()
{
    // ambient
    vec3 ambient = dirlight.ambient * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.4).rgb;

    vec3 norm = normalize(Normal);
    // diffuse
    vec3 lightDir = normalize(-dirlight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirlight.diffuse * diff * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.4).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dirlight.specular * spec * texture(material.specular, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    // result trenutno sadrzi samo DirLight----------------------------------------

    //novi lightDir u odnosu na poziciju izvora pointlighta
    lightDir = normalize(pointlight.position - FragPos);
    //novi diff jer se koristi drugaciji lightdir
    diff = max(dot(norm, lightDir), 0.0);
    // spekularno je vec definisano

    // opadanje jacine svetlosti
    float distance = length(pointlight.position - FragPos);
    float attenuation = 1.0 / (pointlight.constant + pointlight.linear * distance + pointlight.quadratic * (distance * distance));
    // combine results
    ambient = pointlight.ambient * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.45).rgb;
    diffuse = pointlight.diffuse * diff * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.45).rgb;
    specular = pointlight.specular * spec * mix(texture(material.texture1diffuse, TexCoords), texture(material.texture2diffuse, TexCoords), 0.45).rgb;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    result = result + ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}


// #version 330 core
// out vec4 FragColor;
//
// struct Material {
//     sampler2D texture1diffuse;
//     sampler2D texture1diffuse;
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

// struct SpotLight {
//     vec3 position;
//     vec3 direction;
//     float cutOff;
//     float outerCutOff;
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
// #define NR_POINT_LIGHTS 1
//
// in vec3 FragPos;
// in vec3 Normal;
// in vec2 TexCoords;
//
// uniform vec3 viewPos;
// uniform DirLight dirLight;
// // uniform PointLight pointLights[NR_POINT_LIGHTS];
// // uniform SpotLight spotLight;
// uniform Material material;
//
// // function prototypes
// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
// // vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
// // vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//
// void main()
// {
//     // properties
//     vec3 norm = normalize(Normal);
//     vec3 viewDir = normalize(viewPos - FragPos);
//
//     // == =====================================================
//     // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
//     // For each phase, a calculate function is defined that calculates the corresponding color
//     // per lamp. In the main() function we take all the calculated colors and sum them up for
//     // this fragment's final color.
//     // == =====================================================
//     // phase 1: directional lighting
//     vec3 result = CalcDirLight(dirLight, norm, viewDir);
//     // phase 2: point lights
// //     for(int i = 0; i < NR_POINT_LIGHTS; i++)
// //         result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
// //     // phase 3: spot light
// //     result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
//
//     FragColor = vec4(result, 1.0);
// }
//
// // calculates the color when using a directional light.
// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
// {
// //     vec3 lightDir = normalize(-light.direction);
// //     // diffuse shading
// //     float diff = max(dot(normal, lightDir), 0.0);
// //     // specular shading
// //     vec3 reflectDir = reflect(-lightDir, normal);
// //     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
// //     // combine results
// //     vec3 ambient = light.ambient * texture(material.texture1diffuse, TexCoords).rgb;
// //     vec3 diffuse = light.diffuse * diff * texture(material.texture1diffuse, TexCoords).rgb;
// //     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
// //     return (ambient + diffuse + specular);
//
//
//
//     vec3 ambient = light.ambient * texture(material.texture1diffuse, TexCoords).rgb;
//     vec3 lightDir = normalize(-light.direction);
//     float diff = max(dot(normal, lightDir), 0.0);
//     vec3 diffuse = light.diffuse * diff * texture(material.texture1diffuse, TexCoords).rgb;
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//     return (ambient + diffuse + specular);
//
//
//
//
// }

// calculates the color when using a point light.
// vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
//     vec3 lightDir = normalize(light.position - fragPos);
//     // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // attenuation
//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//     // combine results
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//     ambient *= attenuation;
//     diffuse *= attenuation;
//     specular *= attenuation;
//     return (ambient + diffuse + specular);
// }
//
// // calculates the color when using a spot light.
// vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
//     vec3 lightDir = normalize(light.position - fragPos);
//     // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // attenuation
//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//     // spotlight intensity
//     float theta = dot(lightDir, normalize(-light.direction));
//     float epsilon = light.cutOff - light.outerCutOff;
//     float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//     // combine results
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//     ambient *= attenuation * intensity;
//     diffuse *= attenuation * intensity;
//     specular *= attenuation * intensity;
//     return (ambient + diffuse + specular);
// }
























//------------------------------------------------
// vs kopipasta
//________________________________________________
// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoords;
//
// out vec2 TexCoords;
// out vec3 Normal;
// out vec3 FragPos;
//
// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;
//
// void main()
// {
//     FragPos = vec3(model * vec4(aPos, 1.0));
//     Normal = aNormal;
//     TexCoords = aTexCoords;
//     gl_Position = projection * view * vec4(FragPos, 1.0);
// }

// bez izvora svetlosti
// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec2 aTexCoord;
//
// out vec2 TexCoord;
//
// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;
//
// void main()
// {
// 	gl_Position = projection * view * model * vec4(aPos, 1.0f);
// 	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
// }

// //basic lightning vs
// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
//
// out vec3 FragPos;
// out vec3 Normal;
//
// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;
//
// void main()
// {
//     FragPos = vec3(model * vec4(aPos, 1.0));
//     Normal = aNormal;
//
//     gl_Position = projection * view * vec4(FragPos, 1.0);
// }















