#version 330 core

in vec3 color;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
    vec3 lightPos = vec3(0.f, 10.f, 5.f);
    vec3 lightColor = vec3(1.f, 1.f, 1.f);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (color + diffuse) * color;

    FragColor = texture(skybox, FragPos);
}
