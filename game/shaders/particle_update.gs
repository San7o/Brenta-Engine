#version 330 core

layout(points) in;
layout(points, max_vertices = 1) out;

in vec3 outPosition[];
in vec3 outVelocity[];
in float outTTL[];

out vec3 geomPosition;
out vec3 geomVelocity;
out float geomTTL;

void main() {
    geomPosition = outPosition[0];
    geomVelocity = outVelocity[0];
    geomTTL = outTTL[0];

    EmitVertex();
    EndPrimitive();
}
