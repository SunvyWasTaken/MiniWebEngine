#version 420 core

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoords;

in float bone1Influence;

uniform sampler2D textures;

out vec4 FragColor;

void main()
{
    float ground = 0.5;
    float horizon = 1.0;
    float highGround = 2.0;

    vec3 lightPos = vec3(0.0, 10.0, 5.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float y = FragPos.y;

    vec4 color = texture(textures, texCoords);

    vec3 lighting = color.rgb * diffuse;
    //FragColor = vec4(lighting, color.a);
    if (bone1Influence > 0.01)
        FragColor = vec4(bone1Influence, 1.0, 1.0, 1.0); // Rouge
    else
        FragColor = vec4(0.0, 0.0, 0.0, 1.0); // Blanc ou autre couleur
}
