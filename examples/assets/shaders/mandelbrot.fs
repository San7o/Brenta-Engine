#version 400 core

out vec4 FragColor;
in vec2 texCoords;

uniform vec3 resolution;
uniform int maxIterations = 100;
uniform float zoom;
uniform vec3 offset;
uniform vec3 constant;
uniform bool juliaSet = false;

float mandelbrot()
{
    vec2 startingPosition = vec2(0.2, 0.0);

    // Normalized coordinates
    vec2 uv = (gl_FragCoord.xy - 0.3 * resolution.xy) / (resolution.y * 2.0);
    uv += startingPosition + offset.xy * pow(zoom, 4.0);

    vec2 c = 5.0 * uv - vec2(0.7, 0.0);
    c = c / pow(zoom, 4.0) + vec2(0.4, 0.4);
    vec2 z = vec2(0.0);
    float iter = 0.0;
    float smoothIter = 0.0;
    for (int i = 0; i < maxIterations; i++)
    {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if (length(z) > 2.0)
        {
            smoothIter = iter + 1.0 - log(log(length(z))) / log(2.0);
            return smoothIter / float(maxIterations);
        }
        iter++;
    }
    return 0.0;
}

float julia()
{
    vec2 startingPosition = vec2(0.2, 0.0);

    // Normalized coordinates
    vec2 uv = (gl_FragCoord.xy - 0.3 * resolution.xy) / (resolution.y * 2.0);
    uv += startingPosition + offset.xy;

    dvec2 z = 5.0 * uv - dvec2(0.7, 0.0);
    z = z / pow(zoom, 4.0) + vec2(0.4, 0.4);

    float iterations = 0.0;
    float smoothIter = 0.0;
    while(iterations < maxIterations && length(z) < 2.0)
    {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + constant.xy;
        iterations++;
    }

    if (iterations >= maxIterations)
        return 0.0;
    smoothIter = iterations + 1.0 - log(log(float(length(z)))) / log(2.0);
    return smoothIter / maxIterations;
}

void main()
{
    float col = 0.0;

    if (juliaSet)
        col = julia();
    else
        col = mandelbrot();

    // Gamma correction
    col = pow(col, 0.5);
    col = 1.0 - cos(col * 3.14159265);

    vec3 m = vec3(col, col * 0.5, col * 0.25); // Example of gradient mapping
    FragColor = vec4(m, 1.0);
}
