#version 420 core

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoords;

uniform sampler2D textures;

out vec4 FragColor;

void main()
{
    float ground = 0.5;
    float horizon = 1.0;
    float highGround = 2.0;

    vec3 lightDir = normalize(vec3(-0.5, -1.0, -0.3));
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 norm = normalize(Normal);
    // Pour une light directionnelle, on inverse la direction car elle "vient de" la source
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float y = FragPos.y;

    vec4 color = texture(textures, texCoords);

    vec3 lighting = color.rgb * diffuse;
    FragColor = vec4(lighting, color.a);
}
