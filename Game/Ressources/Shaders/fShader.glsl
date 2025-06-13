#version 330 core

in vec3 color;
in vec3 FragPos;
in vec3 Normal;
in vec2 texCoords;

uniform sampler2D textures[3];

out vec4 FragColor;

void main()
{
    float ground = -1.0;
    float horizon = 1.0;
    float highGround = 2.0;

    float y = FragPos.y;

    vec4 rock = texture(textures[0], texCoords * 4.0);
    vec4 grass = texture(textures[1], texCoords * 4.0);
    vec4 snow = texture(textures[2], texCoords * 4.0);

    vec4 finalColor;

    if (y < ground) {
        finalColor = rock;
    } else if (y < horizon) {
        float t = (y - ground) / (horizon - ground);
        finalColor = mix(grass, rock, t);
    } else if (y < highGround) {
        float t = (y - horizon) / (highGround - horizon);
        finalColor = mix(rock, snow, t);
    } else {
        finalColor = snow;
    }

    FragColor = finalColor;
}
