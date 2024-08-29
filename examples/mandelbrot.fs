#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform vec3 resolution;
uniform int maxIterations = 100;
uniform float zoom;
uniform vec3 offset;

float mandelbrot(vec2 uv)
{
    vec2 c = 4.0 * uv - vec2(0.7, 0.0);
    c = c / pow(zoom, 4.0) - vec2(0.65, 0.45);
    vec2 z = vec2(0.0);
    float iter = 0.0;
    for (int i = 0; i < maxIterations; i++)
    {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if (length(z) > 2.0)
        {
            return iter / float(maxIterations);
        }
        iter++;
    }
    return 0.0;
}

void main()
{
    vec2 startingPosition = vec2(0.2, 0.0);
    vec2 uv = (gl_FragCoord.xy - 0.3 * resolution.xy) / (resolution.y * 2.0) + startingPosition + offset.xy;
    vec3 col = vec3(0.0);

    float m = mandelbrot(uv);
    col.xy += m;
    col = pow(col, vec3(0.45));

    FragColor = vec4(col, 1.0);
}
